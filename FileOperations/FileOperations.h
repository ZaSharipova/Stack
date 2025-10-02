#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>

#include "StructsEnums.h"

#define READ_MODE "r"
#define WRITE_MODE "w"

#define CALL_CHECK_IN_OUT_RETURN(call) \
    read_write_error = (call); \
    if (read_write_error != 0) { \
        return read_write_error; \
    }

ParseErr_t HandleOpenFile(Files *in_out_files);
ParseErr_t HandleCloseFile(Files in_out_files);

#endif //FILE_OPERATIONS_H_