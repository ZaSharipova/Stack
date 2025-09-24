#include <stdio.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

#define DEBUG
#define INIT(name) \
    Stack_Info name = { \
        nullptr, 0, 0, \
        { __FILE__, __func__, #name, __LINE__ } \
    };

int main(void) {
    INIT(stk1);
    StackCtor(&stk1, 1);
    StackPush(&stk1, 10);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);
    Stack_t x = 0;
    StackPop(&stk1, &x);

    //StackDump(stdout, stk1);

    return 0;
}