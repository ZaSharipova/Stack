#include "Canary.h"

#include <assert.h>
#include <stdio.h>

#include "ParseInput.h"

StackErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

#ifdef _CANARY
    Stack_t *stack_ptr = (Stack_t *) calloc ((size_t)stk->capacity + 2, sizeof(*stack_ptr));
    if (stack_ptr == NULL) {
        STACKDUMP(open_log_file, stk, kNoMemory);
        return kNoMemory;
    }

    stk->real_data = stack_ptr;
    stk->data = stack_ptr + 1;
    stk->canary_left = Init_Canary();
    stk->canary_right = Init_Canary();
    stk->real_data[0] = (Stack_t)stk->canary_left;
    stk->real_data[stk->capacity + 1] = (Stack_t)stk->canary_right;

    return kSuccess;
#endif
}

uint8_t RandomNonZeroByte() {
    uint8_t b = 0;
    while (b == 0) {
        b = (uint8_t)(rand() & 0xFF);
    }
    return b;
}

uint32_t Init_Canary(void) {
    srand((unsigned int)time(NULL));
    
    uint32_t result = 0;
    for (int i = 0; i < 4; ++i) {
        result <<= 8;
        result |= RandomNonZeroByte();
    }
    
    return result;
}