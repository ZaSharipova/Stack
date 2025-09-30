#ifndef PARSE_INPUT_H_
#define PARSE_INPUT_H_

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

#define MODE "%d"
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

StackErr_t Parse_Graphics(Stack_Info *stk, FILE *file, FILE *open_log_file, FILE *open_out_file);

#endif //PARSE_INPUT_H_