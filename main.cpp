#include <stdio.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

#define DEBUG
#define INIT(name) \
    Stack_Info name = { \
        nullptr, 0, 0, \
        { __FILE__, __func__, #name, __LINE__ } \
    };

#define CHECK_ERROR(err) \
    if (err != kSuccess){ \
        return err; \
    }

int main(void) {
    INIT(stk1);
    StackErr_t err = kSuccess;
    err = StackCtor(&stk1, 10); //change
    CHECK_ERROR(err);

    StackPush(&stk1, 10);
    CHECK_ERROR(err);

    StackPush(&stk1, 20);
    CHECK_ERROR(err);

    StackPush(&stk1, 30);
    CHECK_ERROR(err);

    Stack_t x = 0;
    StackPop(&stk1, &x);
    CHECK_ERROR(err);

    StackDtor(&stk1);
     CHECK_ERROR(err);

    //StackDump(stdout, stk1);
    return 0;
}