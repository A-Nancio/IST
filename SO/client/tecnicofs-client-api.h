#ifndef TECNICOFS_CLIENT_API_H
#define TECNICOFS_CLIENT_API_H

#include "../server/tecnicofs-api-constants.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h> 

void sendMessage();
int receiveMessageInt();
int receiveMessageChar();

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions);
int tfsDelete(char *filename);
int tfsRename(char *filenameOld, char *filenameNew);
int tfsOpen(char *filename, permission mode);
int tfsClose(int fd);
int tfsRead(int fd, char *buffer, int len);
int tfsWrite(int fd, char *buffer, int len);
int tfsMount(char * address);
int tfsUnmount();

#endif /* TECNICOFS_CLIENT_API_H */
