#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>

#include "StructsEnums.h"

#define READ_MODE "r"
#define WRITE_MODE "w"

extern const char *log_file;

#define CALL_CHECK_IN_OUT(call) \
    read_write_error = (call); \
    if (read_write_error != kNoError) { \
        return read_write_error; \
    }

ParseErr_t Handle_Open_File(Files *in_out_files);
ParseErr_t Handle_Close_File(Files in_out_files);
FILE *Open_File(const char *filename, const char *mode);
ParseErr_t Close_File(FILE *file);

#endif //FILE_OPERATIONS_H_