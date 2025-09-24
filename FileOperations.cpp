#include "FileOperations.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

FILE *open_file(const char *filename, const char *mode) {
    assert(filename != NULL);
    assert(mode     != NULL);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

StackErr_t close_file(FILE *file) {
    assert(file != NULL);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kSuccess;
}