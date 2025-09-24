#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>

enum StackErr_t {
    kSuccess = 0,
    kNegativeSize = -1,
    kNegativeCapacity = -2,
    kErrorOpening = -2,
    kErrorClosing = -3,
    kNoMemory = -4, //TODO rename
    kNullPointer = -5,
    kErrorSize = -6,
    kEmptyStack = -7,
};

typedef int Stack_t;

struct Source_Location_Info {
    const char *file_name;
    const char *func_name;
    const char *var;
    int line;
};

struct Stack_Info {
    Stack_t *data;
    int size;
    int capacity;
    Source_Location_Info *var_info;
};

#endif //STRUCTS_ENUMS_H_