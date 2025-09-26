#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

typedef int Stack_t;

enum StackErr_t {
    kSuccess          =   0,
    kNegativeSize     =  -1,
    kNegativeCapacity =  -2,
    kErrorOpening     =  -3,
    kErrorClosing     =  -4,
    kNoMemory         =  -5,
    kNullPointer      =  -6,
    kErrorSize        =  -7,
    kEmptyStack       =  -8,
    kWrongCanaryLeft  =  -9,
    kWrongCanaryRight = -10,
};

enum ParseErr_t {
    kNoNeedToOpen =  0,
    kOpen,
    kNoType  = -1,
};

struct Source_Location_Info {
    const char *file_name;
    const char *func_name;
    const char *var;
    size_t line;
};

struct Stack_Info {
    Stack_t *data;
    ssize_t size;
    ssize_t capacity;

#ifdef _DEBUG
    Stack_t *real_data;
    Source_Location_Info create_var_info;
    uint32_t canary_left;
    uint32_t canary_right;
#endif
};

enum Realloc_Mode {
    kDoChange,
    kNoChange,
};

#endif //STRUCTS_ENUMS_H_