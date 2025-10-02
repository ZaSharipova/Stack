#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

#include "Config.h"

#define NUMBER_OF_ERRORS 13

enum StackErr_t {
    kErrorEmptyStack       = 1 << 0,
    kErrorStackNullPointer = 1 << 1,
    kSizeError             = 1 << 2,
    kNegativeCapacity      = 1 << 3,
    kNegativeSize          = 1 << 4,
    kWrongCanaryLeft       = 1 << 5,
    kWrongCanaryRight      = 1 << 6,
    kWrongHash             = 1 << 7,
    kNoCallocMemory        = 1 << 8,

    kNoMemory              = 1 << 9,
    kZeroNumber            = 1 << 10,
    kNumberNotWritten      = 1 << 11,
    kNoCommandFound        = 1 << 12,
    kSuccess               = 0,
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
    Source_Location_Info create_var_info;
#endif

#ifdef _HASH
    uint32_t data_hash;
#endif
};

enum Realloc_Mode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
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