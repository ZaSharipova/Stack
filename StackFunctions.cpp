#include "StackFunctions.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "StructsEnums.h"

#define STACKDUMP(file, name, err, file_out) StackDump(file, *name, __func__, __LINE__, __FILE__, err, #name)
#define MY_SPEC "%d"


uint32_t stack_canary_left;
uint32_t stack_canary_right;

StackErr_t StackCtor(Stack_Info *stk, ssize_t value, FILE *file) {
    assert(stk);

    stk->capacity = value;
    if (value < 2) {
        stk->capacity = 2;
    }

    stk->size = 0;

    StackErr_t err = kSuccess;
    //StackErr_t err = CheckError(stk, file);
    // if (err != kSuccess) {
    //     return err;
    // }

#ifdef _DEBUG
    err = MakeCanary(stk, file);
    if (err != kSuccess) {
        return err;
    }
#else
    stk->data = (Stack_t *) calloc ((size_t)value, sizeof(Stack_t));
#endif

    if (stk->data == NULL) {
        STACKDUMP(stdout, stk, kNoMemory, file);
        return kNoMemory;
    }

    return kSuccess;
}

StackErr_t StackPush(Stack_Info *stk, Stack_t value, FILE *file) {
    assert(stk);

    StackErr_t err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk, file);
        if (err != kSuccess) {
            return err;
        }
    }

    stk->data[stk->size++] = value;

    err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackPop(Stack_Info *stk, Stack_t *value, FILE *file) {
    assert(stk);
    assert(value);

    StackErr_t err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    if (stk->size == 0) {
        return kEmptyStack;
    }

    *value = stk->data[--stk->size];

    err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk, file);
        if (err != kSuccess) {
            return err;
        }
    }

    err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackTop(Stack_Info stk, Stack_t *value, FILE *file) {
    assert(value);

    StackErr_t err = CheckError(&stk, file);
    if (err != kSuccess) {
        return err;
    }
    
    if (stk.size == 0) {
        STACKDUMP(stdout, &stk, kEmptyStack, file);
        return kEmptyStack;
    }

    *value = stk.data[stk.size - 1];
    return kSuccess;
}

Realloc_Mode CheckSize(ssize_t size, ssize_t *capacity) { //если размеры очень большие
    assert(capacity);

    if (size * INCREASE_VALUE > *capacity) {
        *capacity *= INCREASE_VALUE;
        return kDoChange;

    } else if (size * DECREASE_VALUE < *capacity && size != 0 && *capacity >= 4) {
        *capacity /= DECREASE_VALUE;
        return kDoChange;
    }

    if (*capacity == 0) {
        *capacity = 2;
        return kDoChange;
    }

    return kNoChange;
}

StackErr_t StackRealloc(Stack_Info *stk, FILE *file) {
    assert(stk);

    // StackErr_t err = CheckError(stk, file);
    // if (err != kSuccess) {
    //     return err;
    // }

#ifdef _DEBUG
    size_t new_elems = (size_t)stk->capacity + 2;
#else
    size_t new_elems = (size_t)stk->capacity;
#endif

    Stack_t *realloc_ptr = (Stack_t *) realloc(stk->real_data, ((size_t)new_elems) * sizeof(*realloc_ptr));
    if (realloc_ptr == NULL) {

        STACKDUMP(stdout, stk, kNoMemory, file);
        return kNoMemory;
    }

#ifdef _DEBUG
    stk->real_data = realloc_ptr;
    stk->data = realloc_ptr + 1;
    realloc_ptr[0] = (Stack_t)stk->canary_left; 
    realloc_ptr[stk->capacity + 1] = (Stack_t)stk->canary_right;
#else
    stk->data = realloc_ptr;
#endif

    StackErr_t err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackDtor(Stack_Info *stk, FILE *file) {
    assert(stk);

    StackErr_t err = CheckError(stk, file);
    if (err != kSuccess) {
        return err;
    }

    free(stk->real_data);
    stk->size = -1;
    stk->capacity = -1;

#ifdef _DEBUG
        stk->create_var_info = {NULL, NULL, 0};
#endif

    return kSuccess;
}

Stack_t StackDump(FILE *file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *name) {
    assert(file);
    assert(func_name);
    assert(file_from);
    assert(name);

    fprintf(file, "from %s, function %s: line %d\n", file_from, func_name, line);
    fprintf(file, "error = %s\n", GetErrorString(err)); 
    fprintf(file, "Stack name: %s\n", name);

#ifdef _DEBUG
        fprintf(file, "Stack made: %s, function %s: line %zd\n", stk.create_var_info.file_name, \
        stk.create_var_info.func_name, stk.create_var_info.line);
#endif

    fprintf(file, "STACK[%p] {\n", stk.data);
    fprintf(file, "  size = %zd\n", stk.size);
    fprintf(file, "  capacity = %zd\n", stk.capacity);
    fprintf(file, "  data[%p] {\n", stk.data);

    int pos = 0;
    while(pos < stk.size) {
        fprintf(file, "  *[%d] = " MY_SPEC "\n", pos, stk.data[pos]);
        pos++;
    }

    while (pos < stk.capacity) {
        fprintf(file, "   [%d] = POISON\n", pos);
        pos++;
    }

    fprintf(file, "  }\n");
    fprintf(file, "}\n");

#ifdef _DEBUG
    fprintf(file, "  canary_left = %u (real[%d])\n", stk.real_data[0], 0);
    fprintf(file, "  canary_right = %u (real[%zd])\n", 
        stk.real_data[stk.capacity + 1], stk.capacity + 1);
#endif

    return kSuccess;
}

StackErr_t CheckError(Stack_Info *stk, FILE *file) {
    assert(stk);

    if (stk == NULL) {
        STACKDUMP(stdout, stk, kNullPointer, file);
        return kNullPointer;
    }

    if (stk->data == NULL) {
        STACKDUMP(stdout, stk, kNullPointer, file);
        return kNullPointer;
    }

    if (stk->size > stk->capacity) {
        STACKDUMP(stdout, stk, kErrorSize, file);
        return kErrorSize;
    }

#ifdef _DEBUG
    if (stk->create_var_info.func_name == NULL || stk->create_var_info.file_name == NULL || \
    stk->create_var_info.var == NULL) {
        STACKDUMP(stdout, stk, kNullPointer, file);
        return kNullPointer;
    }
#endif

    if (stk->size < 0) {
        STACKDUMP(stdout, stk, kNegativeSize, file);
        return kNegativeSize;
    }

    if (stk->capacity < 0) {
        STACKDUMP(stdout, stk, kNegativeCapacity, file);
        return kNegativeCapacity;
    }
 
#ifdef _DEBUG

    if (stk->canary_left != (uint32_t)stk->real_data[0]) {
        STACKDUMP(stdout, stk, kWrongCanaryLeft, file);
        return kWrongCanaryLeft;
    }

    if (stk->canary_right != (uint32_t)stk->real_data[stk->capacity + 1]) {
        STACKDUMP(stdout, stk, kWrongCanaryRight, file);
        return kWrongCanaryRight;
    }
#endif

    return kSuccess;
}

const char *GetErrorString(StackErr_t err) {
    switch (err) {
        case kSuccess: return "Success";
        case kNegativeSize: return "Negative Size";
        case kNegativeCapacity: return "Negative Capacity";
        case kNullPointer: return "Null pointer";
        case kNoMemory: return "No memory";
        case kErrorSize: return "Size error";
        case kEmptyStack: return "Empty stack";
        case kErrorOpening: return "Open file failed";
        case kErrorClosing: return "Close file failed";
        case kWrongCanaryLeft: return "Wrong canary left value";
        case kWrongCanaryRight: return "Wrong canary right value";
        default: return "Unknown error";
    }
}

StackErr_t MakeCanary(Stack_Info *stk, FILE *file) {
    assert(stk);

    Stack_t *stack_ptr = (Stack_t *) calloc((size_t)stk->capacity + 2, sizeof(*stack_ptr));
    if (stack_ptr == NULL) {
        STACKDUMP(stdout, stk, kNoMemory, file);
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
}

uint32_t Init_Canary(void) {
    srand((unsigned int)time(NULL));
    return ((uint32_t)rand() << 16) | (uint32_t)rand();
}