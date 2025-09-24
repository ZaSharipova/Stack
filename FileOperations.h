#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>

#include "StructsEnums.h"

FILE *Open_File(const char *filename, const char *mode);
StackErr_t Close_File(FILE *file);

#endif //FILE_OPERATIONS_H_