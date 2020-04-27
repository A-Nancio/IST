# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2019-20

CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -pthread -I../
LDFLAGS=-lm -pthread
MUTEX = -DMUTEX
RWLOCK = -DRWLOCK

# A phony target is one that is not really the name of a file
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: all clean run

all: tecnicofs-rwlock

tecnicofs-rwlock: lib/bst.o lib/inodes.o lib/files.o fs.o-rwlock main.o-rwlock
	$(LD) $(CFLAGS) $(LDFLAGS) $(MUTEX) -o tecnicofs-rwlock lib/bst.o lib/inodes.o lib/files.o lib/hash.o fs.o main.o 


lib/bst.o: lib/bst.c lib/bst.h
	$(CC) $(CFLAGS) -o lib/bst.o -c lib/bst.c

lib/hash.o: lib/hash.c lib/hash.h
	$(CC) $(CFLAGS) -o lib/hash.o -c lib/hash.c

lib/inodes.o: lib/inodes.c lib/inodes.h
	$(CC) $(CFLAGS) -o lib/inodes.o -c lib/inodes.c

lib/files.o: lib/files.c lib/files.h
	$(CC) $(CFLAGS) -o lib/files.o -c lib/files.c

#RWLOCK
fs.o-rwlock: fs.c fs.h lib/bst.h lib/hash.o lib/inodes.o
	$(CC) $(CFLAGS) $(RWLOCK) -o fs.o -c fs.c
main.o-rwlock: main.c fs.h lib/bst.h lib/inodes.h
	$(CC) $(CFLAGS) $(RWLOCK) -o main.o -c main.c


clean:
	@echo Cleaning...
	rm -f lib/*.o *.o tecnicofs-nosync tecnicofs-rwlock tecnicofs-mutex

run: tecnicofs
	./tecnicofs

