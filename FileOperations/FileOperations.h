#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>

#include "StructsEnums.h"
extern const char *log_file;

StackErr_t Handle_Open_File(Files *in_out_files);
StackErr_t Handle_Close_File(Files in_out_files);
FILE *Open_File(const char *filename, const char *mode);
StackErr_t Close_File(FILE *file);

#endif //FILE_OPERATIONS_H_