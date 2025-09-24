#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>
#include "StructsEnums.h"

FILE *open_file(const char *filename, const char *mode);
StackErr_t close_file(FILE *file);

#endif //FILE_OPERATIONS_H_