#include "Canary.h"

#include <assert.h>
#include <stdio.h>

#include "ParseInput.h"

uint32_t stack_canary_left;
uint32_t stack_canary_right;

StackErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);


#ifdef _DEBUG

    Stack_t *stack_ptr = (Stack_t *) calloc((size_t)stk->capacity + 2, sizeof(*stack_ptr));
    if (stack_ptr == NULL) {
        STACKDUMP(open_log_file, stk, kNoMemory);
        return kNoMemory;
    }

    stack_canary_left = Init_Canary();
    stack_canary_right = Init_Canary();

    stk->real_data = stack_ptr;
    stk->data = stack_ptr + 1;
    stk->canary_left = stack_canary_left;
    stk->canary_right = stack_canary_right;
    stk->real_data[0] = (Stack_t)stk->canary_left;
    stk->real_data[stk->capacity + 1] = (Stack_t)stk->canary_right;

    return kSuccess;
#endif
}

uint32_t Init_Canary(void) {
    srand((unsigned int)time(NULL));
    return ((uint32_t)rand() << 16) | (uint32_t)rand();
}