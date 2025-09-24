#include <stdio.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

#define DEBUG
// #define INIT(name) struct Stack_Info name = {.Source_Location_Info.func_name = __func__, .Source_Location_Info.file_name = __FILE__,\
// .Source_Location_Info.var = #name, .Source_Location_Info.line = __LINE__};

int main(void) {
    //INIT(stk1);
    struct Stack_Info stk1 = {};
    StackCtor(&stk1, 1);
    StackPush(&stk1, 10);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);
    Stack_t x = 0;
    StackPop(&stk1, &x);

    //StackDump(stdout, stk1);

    return 0;
}