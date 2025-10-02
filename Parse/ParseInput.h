#ifndef PARSE_INPUT_H_
#define PARSE_INPUT_H_

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

#define PUSH "PUSH"
#define ADD "ADD"
#define POP "POP"
#define SUB "SUB"
#define MUL "MUL"
#define DIV "DIV"
#define SQRT "SQRT"
#define HLT "HLT"
#define OUT "OUT"

#define CHECK_STACK_RETURN(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }

#define CHECK_STACK_FREE_RETURN(call) \
    err = (call); \
    if (err != kSuccess) { \
        free(command); \
        return err; \
    }

StackErr_t ParseGraphics(Stack_Info *stk, FILE *file, FILE *open_log_file, FILE *open_out_file);

#endif //PARSE_INPUT_H_