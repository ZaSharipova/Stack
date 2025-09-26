#ifndef PARSE_INPUT_H_
#define PARSE_INPUT_H_

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

#define CALL_CHECK(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }

StackErr_t parse_graphics(Stack_Info *stk, FILE *file);

#endif //PARSE_INPUT_H_