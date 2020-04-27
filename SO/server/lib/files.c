#include "files.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "../tecnicofs-api-constants.h"


/*
 * Initializes the open_file table and the mutex.
 */
open_file* open_file_table_init(){
    open_file* open_file_table = malloc(sizeof(open_file)*MAX_OPEN_FILES);

    for(int i = 0; i < MAX_OPEN_FILES; i++){
        open_file_table[i].inumber = -1;
        open_file_table[i].mode = 0;
    }
    return open_file_table;
}

/*
 * Releases the allocated memory for the open_file table
 * and destroys the mutex.
 */
void open_file_table_destroy(open_file* open_file_table){
    free(open_file_table);
}

int get_free_index(open_file* open_file_table){
    int i;
    for(i = 0; i < MAX_OPEN_FILES; i++){
        if(open_file_table[i].inumber == -1) return i;
    }
    return -1;
}

//1-found file 0-file not found
int open_file_lookup(open_file* open_file_table, int inumber){
    int i;
    for(i = 0; i<MAX_OPEN_FILES; i++){
        if(open_file_table[i].inumber == inumber) return 1;
    }
    return 0;
}

int open_file_open(open_file* open_file_table, int inumber, int mode){
    int free_index;

    if(open_file_lookup(open_file_table, inumber)) return -1; //FILE IS ALREADY OPENED

    if((free_index = get_free_index(open_file_table)) == -1) return -2; //OPEN_FILE_TABLE IS FULL OF OPENED FILES

    open_file_table[free_index].inumber = inumber;
    open_file_table[free_index].mode = mode;

    return free_index; //FILE IS NOW OPEN- return the fd
}

int open_file_close(open_file* open_file_table, int fd){
    if(fd < 0 || fd > MAX_OPEN_FILES) return -1; //FD NOT VALID
    if(open_file_table[fd].inumber == -1) return -1; //FILE IS NOT OPENED

    open_file_table[fd].inumber = -1;
    open_file_table[fd].mode = 0;

    return 0;
}

int open_file_get(open_file* open_file_table, int fd, int *mode, int *inumber){
    if(fd < 0 || fd > MAX_OPEN_FILES) return -1; //FD NOT VALID

    if(mode)
        *mode = open_file_table[fd].mode;
    if(inumber)
        *inumber = open_file_table[fd].inumber;

    return 0;
}


