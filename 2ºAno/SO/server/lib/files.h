#include "../tecnicofs-api-constants.h"
#include "inodes.h"

#define MAX_OPEN_FILES 5

typedef struct open_file {
    int inumber;
    int mode;
} open_file;

open_file* open_file_table_init();
int open_file_open(open_file* open_file_table, int inumber, int mode);
int open_file_close(open_file* open_file_table, int fd);
void open_file_table_destroy(open_file* open_file_table);
int get_free_index(open_file* open_file_table);
int open_file_lookup(open_file* open_file_table, int inumber);
int open_file_get(open_file* open_file_table, int fd, int *mode, int *inumber);





