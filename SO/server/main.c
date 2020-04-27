#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "lib/timer.h"
#include "fs.h"

#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 128
#define MAX_CLIENTS 5

int numberBuckets = 0;
tecnicofs* fs;
    
int sockfd;

int END = 0;

int sockfd, servlen;
struct sockaddr_un serv_addr;


static void displayUsage (const char* appName){
    printf("Usage: %s socketname outputfile numbuckets\n", appName);
    
}

static void parseArgs (long argc, char* const argv[]){
    if (argc != 4) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
        exit(0);
    }
    numberBuckets = atoi(argv[3]);
}       

int applyCommands(char *line, int user, open_file* file_table, char* buffer){ 	
    char token, arg1[MAX_INPUT_SIZE], arg2[MAX_INPUT_SIZE];
    char aux[MAX_INPUT_SIZE];
    int permissions, return_val;

    sscanf(line, "%c %s %[^\t\n]", &token, arg1, arg2);

    switch (token)   {
        case 'c':
            permissions = atoi(arg2);
            return_val = create(fs, arg1, user, permissions / 10, permissions % 10);
            sprintf(buffer, "%d", return_val);
            break;
        case 'd':
            return_val = delete(fs, file_table, arg1, user); 
            sprintf(buffer, "%d", return_val);
            break;
        case 'r':
            return_val = renameFile(fs, arg1, arg2, user); 
            sprintf(buffer, "%d", return_val);
            break;
        case 'o':
            return_val = openFile(fs, file_table, arg1, atoi(arg2), user);  
            sprintf(buffer, "%d", return_val);
            break;
        case 'x':
            return_val = closeFile(file_table, atoi(arg1));   
            sprintf(buffer, "%d", return_val);
            break;
        case 'l':
            return_val = readFile(fs, file_table, atoi(arg1), aux, atoi(arg2));   
            
            //if succesful
            if (return_val > 0)
                sprintf(buffer, "%d %s", return_val, aux);
            else
                sprintf(buffer, "%d", return_val);
            return 0; //write CHAR in the socket
            break;
        case 'w':
            return_val = writeFileContents(fs, file_table, atoi(arg1), arg2, strlen(arg2));
            sprintf(buffer, "%d", return_val);
            break;
        default: { /* error */
            fprintf(stderr, "Error: command to apply\n");    
            exit(EXIT_FAILURE);      
        }
    }	
    return 1; //write INT in the socket

}

void write_output_file(char* fileName){
    FILE *outputFile;

    outputFile = fopen(fileName ,"w");
    if (outputFile == NULL) {
        perror("Error opening output file");
    }
    print_tecnicofs_tree(outputFile, fs);

    fclose(outputFile);
}


void *str_echo(void *sockfd){
    int n, returnValue, writeInt;
    socklen_t len;
    sigset_t set;
    char line[MAX_INPUT_SIZE];
    char buffer[MAX_INPUT_SIZE];

    struct ucred ucred;
    len = sizeof(struct ucred);

    //Block SIGINT in this thread
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    open_file* file_table = init_open_file_table();

    for (;;){
        /* Lê uma linha do socket */
        n = read(*(int*)sockfd, line, MAX_INPUT_SIZE);
        if (n == 0) return NULL;
        else if (n < 0) perror("str_echo: readline error");
        else{
            line[n] = '\0';

            if (getsockopt(*(int*)sockfd, SOL_SOCKET, SO_PEERCRED, &ucred, &len) == -1){
                perror("str_echo: getUID error");
            }

            writeInt = applyCommands(line, ucred.pid, file_table, buffer);
            
        }
        if (writeInt) {
            returnValue = atoi(buffer);
        	if (write(*(int*)sockfd, &returnValue, sizeof(int)) != sizeof(int))
        		perror("str_echo: write error");
        }
        else{
            n = strlen(buffer);
            if(write(*(int*)sockfd, &buffer, n) != n)
                perror("str_echo: write error");
        }
    }

    close(*(int*)sockfd); 
    destroy_open_file_table(file_table);
}

void mountSocket(char* socketName){
    /* Cria socket stream */
    if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0) ) < 0)
        perror("server: can't open stream socket");

    /* Elimina o nome, para o caso de já existir.*/
    unlink(socketName);
    
    /* O nome serve para que os clientes possam identificar o servidor */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, socketName);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
        perror("server: can't bind local address");

    listen(sockfd, 5);
}

void sig_hand(int sig){
    END = 1;
}

void receiveClients(){
    struct sockaddr_un cli_addr;
    int i, totalThreadsCreated = 0;
    socklen_t clilen;
    pthread_t *clientThreads = malloc(MAX_CLIENTS * sizeof(pthread_t));

    struct sigaction act;

    /* Funcao que da handle do signal */
	act.sa_sigaction = (void *)&sig_hand;
 
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &act, NULL);

    for (i = 0; i < MAX_CLIENTS; i = (i + 1) % MAX_CLIENTS){
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (END) break;

        if (newsockfd < 0) perror("server: accept error");
        
        if (pthread_create(&clientThreads[i], NULL, str_echo, (void *) &newsockfd) != 0){
            perror("receiveClients: failed to create thread\n");
        }
        totalThreadsCreated++;
    }

    if(totalThreadsCreated > MAX_CLIENTS) totalThreadsCreated = MAX_CLIENTS;

    for(int x = 0; x < totalThreadsCreated; x++){
        if (pthread_join(clientThreads[x], NULL) != 0) perror("receiveClients: failed to join thread");
    }

    free(clientThreads);

}

int main(int argc, char* argv[]) {
    TIMER_T beginTime, endTime; 
    parseArgs(argc, argv);
    fs = new_tecnicofs(numberBuckets);
    mountSocket(argv[1]);

    TIMER_READ(beginTime);
    receiveClients();
    TIMER_READ(endTime);
    printf("TecnicoFS completed in %0.4f seconds.\n", TIMER_DIFF_SECONDS(beginTime, endTime));
    
    write_output_file(argv[2]);
    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);
}


