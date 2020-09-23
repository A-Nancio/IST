#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

tecnicofs* new_tecnicofs(int numberBuckets){
	int i;
	tecnicofs* fs = malloc(sizeof(tecnicofs));

	if (!fs) {
		perror("failed to allocate tecnicofs");
	}

	fs->fs_nodes = malloc(sizeof(tecnicofs_node*)*numberBuckets);

	for(i = 0; i < numberBuckets; i++){
		fs->fs_nodes[i] = new_tecnicofs_node();
	}

	fs->numberBuckets = numberBuckets;

	inode_table_init();

	return fs;
}

tecnicofs_node* new_tecnicofs_node(){
	tecnicofs_node* fs_node = malloc(sizeof(tecnicofs_node));
	if (!fs_node) {
		perror("failed to allocate tecnicofs_node");

	}
	fs_node->bstRoot = NULL;
	init_lock(fs_node);
	return fs_node;
}

void init_lock(tecnicofs_node* fs_node){
	#ifdef MUTEX
	if (pthread_mutex_init(&fs_node->mutex_lock, NULL) != 0)
    {
        perror("\n mutex init failed\n");
    }
	#elif RWLOCK
	if (pthread_rwlock_init(&fs_node->rw_lock, NULL) != 0)
    {
        perror("\n rwlock init failed\n");
    }
	#endif
}

void destroy_lock(tecnicofs_node* fs_node){
	#ifdef MUTEX
    	if (pthread_mutex_destroy(&fs_node->mutex_lock) != 0){
            perror("mutex_destroy failed\n");
        }
    #elif RWLOCK
        if (pthread_rwlock_destroy(&fs_node->rw_lock) != 0){
        	perror("mutex_destroy failed\n");
        }
    #endif
}

void thread_fs_lock(tecnicofs_node* fs_node, int n){
	#ifdef MUTEX
		if(pthread_mutex_lock(&fs_node->mutex_lock)){ 
			perror("Mutex lock error\n");
		}
	
	#elif RWLOCK
		if (n){
			if(pthread_rwlock_wrlock(&fs_node->rw_lock)){
				perror("RWLOCK lock error\n"); 
			}
		}	
		else{ 
			if(pthread_rwlock_rdlock(&fs_node->rw_lock)){
				perror("RWLOCK lock\n"); 
			}
		}
	#endif
}

//return 1 if locked
int thread_fs_trylock(tecnicofs_node* fs_node){
	#ifdef MUTEX
	if(pthread_mutex_trylock(&fs_node->mutex_lock)){
		return 0;
	}
	#elif RWLOCK
	if(pthread_rwlock_trywrlock(&fs_node->rw_lock)){
		return 0;
	}
	#endif
	return 1;
}

void thread_fs_unlock(tecnicofs_node* fs_node){
	#ifdef MUTEX
	if (pthread_mutex_unlock(&fs_node->mutex_lock)) perror("MUTEX unlock error\n");
	#elif RWLOCK
	if (pthread_rwlock_unlock(&fs_node->rw_lock)) perror("RWLOCK unlock error\n");
	#endif
}


void free_tecnicofs(tecnicofs* fs){
	int i;
	tecnicofs_node* fs_node;

	inode_table_destroy();
	for(i = 0; i<fs->numberBuckets; i++){
		fs_node = fs->fs_nodes[i];
		destroy_lock(fs_node);
		free_tree(fs_node->bstRoot);
		free(fs_node);
	}
	free(fs->fs_nodes);
	free(fs);
	
}

tecnicofs_node* get_node(tecnicofs* fs, char *name){
	int hash_number = hash(name, fs->numberBuckets);
	tecnicofs_node* node = fs->fs_nodes[hash_number]; 
	return node;

}

int create(tecnicofs* fs, char *name, uid_t user, permission ownerPerm, permission othersPerm){
	if (lookup(fs, name) != -1) return TECNICOFS_ERROR_FILE_ALREADY_EXISTS; //ERRO JA EXISTE FICHEIRO

	tecnicofs_node* fs_node = get_node(fs, name);

	int inumber = inode_create(user, ownerPerm, othersPerm);
	thread_fs_lock(fs_node, 1);
	fs_node->bstRoot = insert(fs_node->bstRoot, name, inumber);
	thread_fs_unlock(fs_node);
	
	return 0;
}

int delete(tecnicofs* fs, open_file *open_file_table, char *name, uid_t user){
	int inumber, fd;
	uid_t owner = 1;

	if ((inumber = lookup(fs, name)) == -1) return TECNICOFS_ERROR_FILE_NOT_FOUND; //ERRO NAO EXISTE FICHEIRO

	inode_get(inumber, &owner, NULL, NULL, NULL, 1); //get owner

	if(owner != user) return TECNICOFS_ERROR_PERMISSION_DENIED; //ERRO FICHEIRO NAO PERTENCE AO USER
	
	fd = open_file_lookup(open_file_table, inumber);
	open_file_close(open_file_table, fd);
	
	inode_delete(inumber);
	tecnicofs_node* fs_node = get_node(fs, name);

	thread_fs_lock(fs_node, 1);
	fs_node->bstRoot = remove_item(fs_node->bstRoot, name);
	thread_fs_unlock(fs_node);

	return 0;
}

int lookup(tecnicofs* fs, char *name){
	tecnicofs_node* fs_node = get_node(fs, name);

	thread_fs_lock(fs_node, 0);
	node* searchNode = search(fs_node->bstRoot, name);
	thread_fs_unlock(fs_node);

	if ( searchNode ) return searchNode->inumber;
	return -1;
}

int tryLockBoth(tecnicofs_node* node1, tecnicofs_node* node2, int numberAttempts){
	int lock1, lock2;

	lock1 = thread_fs_trylock(node1);

	if (node1 != node2){
		lock2 = thread_fs_trylock(node2);
		if (lock1 && lock2) return 1;
		else{
			if (lock1) thread_fs_unlock(node1);
			if (lock2) thread_fs_unlock(node2);
			int delay = rand() % (numberAttempts + 1);
			usleep(delay * 1000);
			return 0;
		} 	
	}
	else return lock1;

}

int renameFile(tecnicofs *fs, char* name, char* new_name, uid_t user){
	int numberAttempts = 0;
	tecnicofs_node* node_name = get_node(fs, name);
	tecnicofs_node* node_newName = get_node(fs, new_name);

	while(!tryLockBoth(node_name, node_newName, numberAttempts)){
		numberAttempts++;
	}
	
	node* searchNode = search(node_name->bstRoot, name);
	
	if (searchNode != NULL){
		int inumber = searchNode->inumber;
		uid_t owner = 1;

		inode_get(inumber, &owner, NULL, NULL, NULL, 1); //get owner
		if (user != owner) return TECNICOFS_ERROR_PERMISSION_DENIED; //ERRO FICHEIRO NAO PERTENCE AO USER

		if (search(node_newName->bstRoot, new_name) == NULL){
			node_name->bstRoot = remove_item(node_name->bstRoot, name);
			node_newName->bstRoot = insert(node_newName->bstRoot, new_name, inumber);
		}
		else printf("%s already exists\n", new_name);
	}
	else printf("%s file to rename not found\n", name);
	
	if (node_name == node_newName) thread_fs_unlock(node_name);
	else{	
		thread_fs_unlock(node_name);
		thread_fs_unlock(node_newName);
	}

	return 0;
}

int check_perms(uid_t user, int mode, int inumber){
	uid_t owner = 1;
	permission ownerPerm, othersPerm;
	inode_get(inumber, &owner, &ownerPerm, &othersPerm, NULL, 1); //get owner
	
	if(user==owner){
		if(ownerPerm != mode && ownerPerm != RW) return -1;
	} 
	else
		if(othersPerm != mode && othersPerm != RW) return -1;
	
	return 0;
	
}
//perms 0-none 1-wronly 2-rdonly 3-rdwr
int openFile(tecnicofs *fs,open_file* open_file_table ,char* filename, int mode, uid_t user){
	int inumber, output;
	
	if((inumber = lookup(fs, filename)) == -1) return TECNICOFS_ERROR_FILE_NOT_FOUND; 
	
	if(check_perms(user, mode, inumber) == -1) return TECNICOFS_ERROR_PERMISSION_DENIED;
	
	if((output = open_file_open(open_file_table, inumber, mode)) == -1) return TECNICOFS_ERROR_FILE_IS_OPEN; 

	if(output == -2) return TECNICOFS_ERROR_MAXED_OPEN_FILES;
	
	return output;
}

int closeFile(open_file* open_file_table, int fd){
	if (open_file_close(open_file_table, fd) == -1) return TECNICOFS_ERROR_FILE_NOT_OPEN;
	return 0;
}

int readFile(tecnicofs *fs,open_file* open_file_table ,int fd, char* buffer, int len){
	int mode, inumber, read_len;

	if(open_file_get(open_file_table, fd, &mode, &inumber) == -1) return TECNICOFS_ERROR_OTHER;

	if(mode == 0) return TECNICOFS_ERROR_FILE_NOT_OPEN;

	if(mode != READ && mode != RW) return TECNICOFS_ERROR_INVALID_MODE; 

	read_len = inode_get(inumber, NULL, NULL, NULL, buffer, len - 1);

	if(read_len == -1) return TECNICOFS_ERROR_OTHER; 

	return read_len;
}

int writeFileContents(tecnicofs *fs,open_file* open_file_table, int fd, char* buffer, int len){
	int mode, inumber;

	if(open_file_get(open_file_table, fd, &mode, &inumber) == -1) return TECNICOFS_ERROR_FILE_NOT_FOUND;

	if(mode != WRITE && mode != RW) return TECNICOFS_ERROR_INVALID_MODE; 

	if(inode_set(inumber, buffer, len) == -1) return TECNICOFS_ERROR_OTHER;

	return 0;
}

open_file* init_open_file_table(){
	return open_file_table_init();
}

void destroy_open_file_table(open_file* op){
	open_file_table_destroy(op);
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	int i;
	tecnicofs_node* fs_node;
	for(i = 0; i < fs->numberBuckets; i++){
		fs_node = fs->fs_nodes[i];
		print_tree(fp, fs_node->bstRoot);

	}
}
