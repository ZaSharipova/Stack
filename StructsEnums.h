#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

#define MY_SPEC "%d"

typedef int Stack_t;

enum StackErr_t {
    kSuccess          =   0,
    kNegativeSize     =  -1,
    kNegativeCapacity =  -2,
    kNoMemory         =  -3,
    kNullPointer      =  -4,
    kErrorSize        =  -5,
    kEmptyStack       =  -6,
    kWrongCanaryLeft  =  -7,
    kWrongCanaryRight =  -8,
    kHashMismatch     =  -9,
    kNoCommand        = -10,
    kZeroNumber       = -11,
};

enum ParseErr_t {
    kNoError      = 0,
    kErrorParsing = -1,
    kWrongMode    = -2,
    kErrorOpening = -3,
    kErrorClosing = -4,
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
#endif

#ifdef _CANARY
    uint32_t canary_left;
    uint32_t canary_right;
#endif

#ifdef _HASH
    uint32_t data_hash;
#endif
};

enum Realloc_Mode {
    kDoChange,
    kNoChange,
};

struct Files {
    const char *log_file;
    FILE *open_log_file;
    const char *in_file;
    FILE *open_in_file;
    const char *out_file;
    FILE *open_out_file;
};

#endif //STRUCTS_ENUMS_H_