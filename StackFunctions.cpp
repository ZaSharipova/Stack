#include "StackFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

#define STACKDUMP(file, name, err) StackDump(file, *name, __func__, __LINE__, __FILE__, err, #name)

StackErr_t StackCtor(Stack_Info *stk, int value) {
    assert(stk);

    stk->capacity = value;

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    stk->data = (Stack_t *) calloc((size_t)value, sizeof(Stack_t));
    if (stk->data == NULL) {
        return kNoMemory;
    }

    STACKDUMP(stdout, stk, kSuccess);
    return kSuccess;
}

StackErr_t StackPush(Stack_Info *stk, Stack_t value) {
    assert(stk);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk);
        if (err != kSuccess) {
            return err;
        }
    }

    stk->data[stk->size++] = value;

    err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    STACKDUMP(stdout, stk, kSuccess);
    return kSuccess;
}

StackErr_t StackPop(Stack_Info *stk, Stack_t *value) {
    assert(stk);
    assert(value);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    if (stk->size == 0) {
        return kEmptyStack;
    }

    *value = stk->data[--stk->size];

    err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk);
        if (err != kSuccess) {
            return err;
        }
    }

    STACKDUMP(stdout, stk, kSuccess);
    return kSuccess;
}

StackErr_t StackTop(Stack_Info stk, Stack_t *value) {
    assert(value);

    StackErr_t err = CheckError(&stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, &stk, err);
        return err;
    }
    
    if (stk.size == 0) {
        return kEmptyStack;
    }

    *value = stk.data[stk.size - 1];
    return kSuccess;
}

Realloc_Mode CheckSize(int size, int *capacity) { //если размеры очень большие
    assert(capacity);

    if (size * INCREASE_VALUE > *capacity) {
        *capacity *= INCREASE_VALUE;
        return kIncrease;

    } else if (size * DECREASE_VALUE < *capacity && size != 0 && *capacity >= 4) {
        *capacity /= DECREASE_VALUE;
        return kDecrease;
    }

    if (*capacity == 0) {
        *capacity = 2;
        return kIncrease;
    }

    return kNoChange;
}

StackErr_t StackRealloc(Stack_Info *stk) {
    assert(stk);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    printf("%d\n", stk->capacity);
    Stack_t *realloc_ptr = (Stack_t *) realloc(stk->data, (size_t)stk->capacity * sizeof(Stack_t));
    if (realloc_ptr != NULL) {
        stk->data = realloc_ptr;
    } else {
        free(stk->data); //надо ли
        return kNoMemory;
    }


    err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    return kSuccess;
}

StackErr_t StackDtor(Stack_Info *stk) {
    assert(stk);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        STACKDUMP(stdout, stk, err);
        return err;
    }

    free(stk->data);
    stk->size = -1;
    stk->capacity = -1;
    stk->create_var_info = {NULL, NULL, 0};

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
    fprintf(file, "Stack made: %s, function %s, line: %d\n", stk.create_var_info.file_name, stk.create_var_info.func_name, stk.create_var_info.line);
    fprintf(file, "STACK[%p] {\n", stk.data);
    fprintf(file, "  size = %d\n", stk.size);
    fprintf(file, "  capacity = %d\n", stk.capacity);
    fprintf(file, "  data[%p] {\n", stk.data);

    int pos = 0;
    while(pos < stk.size) {
        fprintf(file, "  *[%d] = %d\n", pos, stk.data[pos]); //TODO what to do with Stack_t
        pos++;
    }

    while (pos < stk.capacity) {
        fprintf(file, "   [%d] = POISON\n", pos); //заполнять Poison всегда
        pos++;
    }

    fprintf(file, "  }\n");
    fprintf(file, "}\n");

    return kSuccess;
}

StackErr_t CheckError(Stack_Info *stk) {
    assert(stk);

    if (stk == NULL) {
        return kNullPointer;
    }

    if (stk->size > stk->capacity) {
        return kErrorSize;
    }

    if (stk->size < 0) {
        return kNegativeSize;
    }

    if (stk->capacity < 0) {
        return kNegativeCapacity;
    }
 
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
        default: return "Unknown error";
    }
}