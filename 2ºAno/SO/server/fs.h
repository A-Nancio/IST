#ifndef FS_H
#define FS_H
#include "lib/bst.h"
#include "lib/hash.h"
#include "lib/inodes.h"
#include "lib/timer.h"
#include "lib/files.h"
#include <pthread.h>

typedef struct tecnicofs_node {
    node* bstRoot;
    pthread_mutex_t mutex_lock;
    pthread_rwlock_t rw_lock;
} tecnicofs_node;

typedef struct tecnicofs {
    tecnicofs_node** fs_nodes;
    int numberBuckets;
} tecnicofs;

tecnicofs* new_tecnicofs(int buckets);
tecnicofs_node* new_tecnicofs_node();
tecnicofs_node* get_node(tecnicofs* fs, char *name);

void free_tecnicofs(tecnicofs* fs);
int create(tecnicofs* fs, char *name, uid_t user, permission ownerPerm, permission othersPerm);
int delete(tecnicofs* fs, open_file *open_file_table, char *name, uid_t user);
int lookup(tecnicofs* fs, char *name);
int renameFile(tecnicofs *fs, char* name, char* new_name, uid_t user);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);
int thread_fs_trylock(tecnicofs_node* fs_node);
int tryLockBoth(tecnicofs_node* node1, tecnicofs_node* node2, int numberAttempts);
void thread_fs_lock(tecnicofs_node* fs_node, int n);
void thread_fs_unlock(tecnicofs_node* fs_node);
void init_lock(tecnicofs_node* fs_node);
void destroy_lock(tecnicofs_node* fs_node);

open_file* init_open_file_table();
void destroy_open_file_table(open_file* op);

int openFile(tecnicofs *fs,open_file* open_file_table ,char* filename, int mode, uid_t user);
int closeFile(open_file* open_file_table, int fd);
int readFile(tecnicofs *fs, open_file* open_file_table ,int fd, char* buffer, int len);
int writeFileContents(tecnicofs *fs,open_file* open_file_table, int fd, char* buffer, int len);

#endif /* FS_H */
