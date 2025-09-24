#include "FileOperations.h"

#include <assert.h>
#include <stdio.h>

#include "StructsEnums.h"

FILE *Open_File(const char *filename, const char *mode) {
    assert(filename != NULL);
    assert(mode     != NULL);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

StackErr_t Close_File(FILE *file) {
    assert(file != NULL);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kSuccess;
}