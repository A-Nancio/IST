#include "../../server/tecnicofs-api-constants.h"
#include "../tecnicofs-client-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s sock_path\n", argv[0]);
        exit(0);
    }
    int out = tfsMount(argv[1]);
    printf("\n%d\n", out);
    assert(out == 0);
    printf("Test: create file sucess");
    assert(tfsCreate("a", RW, READ) == 0);
    printf("Test: create file with name that already exists");
    assert(tfsCreate("b", RW, READ) == 0);
    assert(tfsCreate("c", RW, READ) == 0);
    assert(tfsCreate("d", RW, READ) == 0);
    assert(tfsCreate("e", RW, READ) == 0);
    assert(tfsCreate("a", RW, READ) == TECNICOFS_ERROR_FILE_ALREADY_EXISTS);
    assert(tfsCreate("f", RW, READ) == 0);
    assert(tfsCreate("g", RW, READ) == 0);
    assert(tfsUnmount() == 0);

    return 0;
}