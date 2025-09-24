#include "StackFunctions.h"

#include <stdio.h>
#include "assert.h"
#include "StructsEnums.h"

#define STACKDUMP(file, name, err) StackDump(file, *name, __func__, __LINE__, __FILE__, err, #name)

StackErr_t StackCtor(Stack_Info *stk, Stack_t value) {
    assert(stk);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        perror("");
        return err;
    }

    stk->capacity = value;
    if (!stk->capacity) {
        return kNullPointer;
    }

    stk->data = (Stack_t *)calloc((size_t)value, sizeof(Stack_t));
    if (stk->data == NULL) {
        return kNoMemory;
    }

    return kSuccess;
    STACKDUMP(stdout, stk, kSuccess);
}

StackErr_t StackPush(Stack_Info *stk, Stack_t value) {
    assert(stk);
    assert(value);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    if (stk->size * 2 > stk->capacity) {
        err = StackRealloc(stk, '+');
        if (err != kSuccess) {
            return err;
        }
    }
    stk->data[stk->size++] = value;

    err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
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
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    if (stk->capacity == 0) {
        return kEmptyStack;
    }

    *value = stk->data[--stk->size];

    err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    err = StackRealloc(stk, '-');
    if (err != kSuccess) {
        return err;
    }

    STACKDUMP(stdout, stk, kSuccess);
    return kSuccess;
}

StackErr_t StackTop(Stack_Info stk, Stack_t *value) {
    assert(value);

    StackErr_t err = CheckError(&stk);
    if (err != kSuccess) {
        StackDump(stdout, stk, __func__, __LINE__, __FILE__, err, (const char *)&stk);
        return err;
    }
    
    if (stk.capacity == 0) {
        return kEmptyStack;
    }
    
    *value = stk.data[stk.size - 1];
    return kSuccess;
}

StackErr_t StackRealloc(Stack_Info *stk, char mode) {
    assert(stk);
    assert(mode);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    if (mode =='+' && stk->size * 2 > stk->capacity) {
        stk->capacity *= 2;

    } else if (mode == '-' && stk->size * 4 < stk->capacity){
        stk->capacity /= 4;

    } else if (mode == '+' && stk->capacity == 0) {
        stk->capacity = 2;
    
    } else {
        return kSuccess;
    }

    stk->data = (Stack_t *) realloc(stk->data, (size_t)stk->capacity * sizeof(Stack_t));
    if (stk->data == NULL) {
        return kNoMemory;
    }


    err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    return kSuccess;
}

StackErr_t StackDtor(Stack_Info *stk) {
    assert(stk);

    StackErr_t err = CheckError(stk);
    if (err != kSuccess) {
        StackDump(stdout, *stk, __func__, __LINE__, __FILE__, err, (const char *)stk);
        return err;
    }

    free(stk);
    stk->size = -1;
    stk->capacity = -1;
    stk->var_info = NULL;

    return kSuccess;
}

Stack_t StackDump(FILE *file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *name) {
    assert(file);

    fprintf(file, "from %s, function %s: line %d\n", file_from, func_name, line);
    fprintf(file, "STACK[%p] {\n", stk.data);
    fprintf(file, "  size = %d\n", stk.size);
    fprintf(file, "  capacity = %d\n", stk.capacity);
    fprintf(file, "  data[%p]\n", stk.data);
    fprintf(file, "  {\n");

    int pos = 0;
    while(pos < stk.size) {
        fprintf(file, "  *[%d] = %d\n", pos, stk.data[pos]); //TODO what to do with Stack_t
        pos++;
    }
    while (pos < stk.capacity) {
        fprintf(file, "   [%d] = POISON\n", pos);
        pos++;
    }

    fprintf(file, "  }\n");
    fprintf(file, "}\n");

    return kSuccess;
}

StackErr_t CheckError(Stack_Info *stk) {
    if (stk == NULL) {
        return kNullPointer;
    }

    if (stk->size > stk->capacity) {
        return kErrorSize;
    }

    if (stk->size < 0 || stk->capacity < 0) {
        return kErrorSize;
    }

    return kSuccess;
}