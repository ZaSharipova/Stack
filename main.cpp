#include <stdio.h>

#include "StackFunctions.h"
#include "StructsEnums.h"

#define INIT_DEBUG(name) \
    Stack_Info name = { \
        nullptr, 0, 0, \
        { __FILE__, __func__, #name, __LINE__ } \
    };

#define CHECK_ERROR(err) \
    if (err != kSuccess) { \
        return err; \
    }

#define CALL_CHECK(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }

int main(void) {
    #define DEBUG
    Stack_Info stk1 = {};

#ifdef DEBUG
    stk1.create_var_info = {NULL, NULL, NULL, 0};
#endif

    StackErr_t err = kSuccess;
    CALL_CHECK(StackCtor(&stk1, 1));
    CALL_CHECK(StackPush(&stk1, 10));  
    CALL_CHECK(StackPush(&stk1, 20));
    CALL_CHECK(StackPush(&stk1, 30));  

    Stack_t x = 0; 
    CALL_CHECK(StackPop(&stk1, &x));   
    CALL_CHECK(StackDtor(&stk1));

    // err = StackCtor(&stk1, 10); //change
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 10);
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 20);
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 30);
    // CHECK_ERROR(err);

    // Stack_t x = 0;
    // err = StackPop(&stk1, &x);
    // CHECK_ERROR(err);

    // err = StackDtor(&stk1);
    // CHECK_ERROR(err);

    //StackDump(stdout, stk1);
    return 0;
}