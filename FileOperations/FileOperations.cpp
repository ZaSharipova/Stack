#include "FileOperations.h"

#include <assert.h>
#include <stdio.h>

#include "StructsEnums.h"

const char *log_file = NULL;

StackErr_t Handle_Open_File(Files *in_out_files) {
    assert(in_out_files);

    if (log_file != NULL) {
        in_out_files->open_log_file = Open_File(log_file, "w");
        if (in_out_files->open_log_file == NULL) {
            return kErrorOpening;
        }
    } else {
        in_out_files->open_log_file = stdout;
    }

    if (in_out_files->in_file != NULL) {
        in_out_files->open_in_file = Open_File(in_out_files->in_file, "r");
        if (in_out_files->open_in_file == NULL) {
            return kErrorOpening;
        }
    } else {
        in_out_files->open_in_file = stdin;
    }

    if (in_out_files->out_file != NULL) {
        in_out_files->open_out_file = Open_File(in_out_files->out_file, "w");
        if (in_out_files->open_out_file == NULL) {
            return kErrorOpening;
        }
    } else {
        in_out_files->open_out_file = stdout;
    }

   return kSuccess;
}

StackErr_t Handle_Close_File(Files in_out_files) {

    if (log_file != NULL) {
        StackErr_t err = kSuccess;
        err = Close_File(in_out_files.open_log_file);
        if (err != kSuccess) {
            return err;
        }

    }

    if (in_out_files.open_in_file != stdin) {
        StackErr_t err = kSuccess;
        err = Close_File(in_out_files.open_in_file);
        if (err != kSuccess) {
            return err;
        }
    }

    if (in_out_files.open_out_file != stdout) {
        StackErr_t err = kSuccess;
        err = Close_File(in_out_files.open_out_file);
        if (err != kSuccess) {
            return err;
        }
    }

   return kSuccess;
}

FILE *Open_File(const char *filename, const char *mode) {
    assert(filename);
    assert(mode);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

StackErr_t Close_File(FILE *file) {
    assert(file);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kSuccess;
}