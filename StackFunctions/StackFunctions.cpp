#include "StackFunctions.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "Canary.h"
#include "StructsEnums.h"
#include "StackHash.h"
#include "FileOperations.h"

//TODO clean POISON

StackErr_t StackCtor(Stack_Info *stk, ssize_t value, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    stk->capacity = value;
    if (value < 0) {
        STACKDUMP(open_log_file, stk, kNegativeCapacity);
        return kNegativeCapacity;
    }

    stk->size = 0;

    StackErr_t err = kSuccess;
    //StackErr_t err = CheckError(stk, open_log_file);
    // if (err != kSuccess) {
    //     return err;
    // }

#ifdef _DEBUG
    err = MakeCanary(stk, open_log_file);
    if (err != kSuccess) {
        STACKDUMP(open_log_file, stk, kNoMemory);
        return err;
    }
    update_data_hash(stk);
#else
    stk->data = (Stack_t *) calloc ((size_t)value, sizeof(Stack_t));
#endif

    if (stk->data == NULL) {
        STACKDUMP(open_log_file, stk, kNoMemory);
        return kNoMemory;
    }

    return kSuccess;
}

StackErr_t StackPush(Stack_Info *stk, Stack_t value, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    StackErr_t err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk, open_log_file);
        if (err != kSuccess) {
            return err;
        }
    }

    stk->data[stk->size++] = value;
    update_data_hash(stk);

    err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackPop(Stack_Info *stk, Stack_t *value, FILE *open_log_file) {
    assert(stk);
    assert(value);
    assert(open_log_file);

    StackErr_t err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    if (stk->size == 0) {
        return kEmptyStack;
    }

    *value = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    update_data_hash(stk);

    err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    Realloc_Mode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        err = StackRealloc(stk, open_log_file);
        if (err != kSuccess) {
            return err;
        }
    }

    err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackTop(Stack_Info stk, Stack_t *value, FILE *open_log_file) {
    assert(value);
    assert(open_log_file);

    StackErr_t err = CheckError(&stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }
    
    if (stk.size == 0) {
        STACKDUMP(open_log_file, &stk, kEmptyStack);
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

StackErr_t StackRealloc(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    // StackErr_t err = CheckError(stk, open_log_file);
    // if (err != kSuccess) {
    //     return err;
    // }

#ifdef _DEBUG
    size_t new_elems = (size_t)stk->capacity + 2;
    Stack_t *ptr = stk->real_data;
#else
    size_t new_elems = (size_t)stk->capacity;
    Stack_t *ptr = stk->data;
#endif

    Stack_t *realloc_ptr = (Stack_t *) realloc(ptr, ((size_t)new_elems) * sizeof(*realloc_ptr));
    if (realloc_ptr == NULL) {
        STACKDUMP(open_log_file, stk, kNoMemory);
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

    StackErr_t err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    return kSuccess;
}

StackErr_t StackDtor(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    StackErr_t err = CheckError(stk, open_log_file);
    if (err != kSuccess) {
        return err;
    }

    stk->size = -1;
    stk->capacity = -1;

#ifdef _DEBUG
        free(stk->real_data);
        stk->create_var_info = {NULL, NULL, 0};
        stk->data_hash = 0;
#endif

    return kSuccess;
}

Stack_t StackDump(FILE *open_log_file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *stk_name) {
    assert(open_log_file);
    assert(func_name);
    assert(file_from);
    assert(stk_name);

    fprintf(open_log_file, "from %s, function %s: line %d\n", file_from, func_name, line);
    fprintf(open_log_file, "error = %s%s%s\n", RED(open_log_file), GetErrorString(err), RESET(open_log_file)); 
    fprintf(open_log_file, "Stack name: %s\n", stk_name);

#ifdef _DEBUG
        fprintf(open_log_file, "Stack made: %s, function %s: line %zd\n", stk.create_var_info.file_name, \
        stk.create_var_info.func_name, stk.create_var_info.line);
#endif

    fprintf(open_log_file, "STACK[%p] {\n", stk.data);
    fprintf(open_log_file, "  size = %zd\n", stk.size);
    fprintf(open_log_file, "  capacity = %zd\n", stk.capacity);
    fprintf(open_log_file, "  data[%p] {\n", stk.data);

    int pos = 0;
    while(pos < stk.size) {
        fprintf(open_log_file, "  *[%d] = " MY_SPEC "\n", pos, stk.data[pos]);
        pos++;
    }

    while (pos < stk.capacity) {
        fprintf(open_log_file, "   [%d] = POISON\n", pos);
        pos++;
    }

    fprintf(open_log_file, "  }\n");
    fprintf(open_log_file, "}\n");

#ifdef _DEBUG
    if (stk.capacity >= 0) {
        fprintf(open_log_file, "  canary_left = %u (real[%d])\n", stk.real_data[0], 0);
        fprintf(open_log_file, "  canary_right = %u (real[%zd])\n", 
            stk.real_data[stk.capacity + 1], stk.capacity + 1);
    }
#endif

    return kSuccess;
}

StackErr_t CheckError(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    if (stk == NULL) {
        STACKDUMP(open_log_file, stk, kNullPointer);
        return kNullPointer;
    }

    if (stk->data == NULL) {
        STACKDUMP(open_log_file, stk, kNullPointer);
        return kNullPointer;
    }

    if (stk->size > stk->capacity) {
        STACKDUMP(open_log_file, stk, kErrorSize);
        return kErrorSize;
    }

#ifdef _DEBUG
    if (stk->create_var_info.func_name == NULL || stk->create_var_info.file_name == NULL || \
    stk->create_var_info.var == NULL) {
        STACKDUMP(open_log_file, stk, kNullPointer);
        return kNullPointer;
    }
#endif

    if (stk->size < 0) {
        STACKDUMP(open_log_file, stk, kNegativeSize);
        return kNegativeSize;
    }

    if (stk->capacity < 0) {
        STACKDUMP(open_log_file, stk, kNegativeCapacity);
        return kNegativeCapacity;
    }
 
#ifdef _DEBUG

    if (stk->canary_left != (uint32_t)stk->real_data[0]) {
        STACKDUMP(open_log_file, stk, kWrongCanaryLeft);
        return kWrongCanaryLeft;
    }

    if (stk->canary_right != (uint32_t)stk->real_data[stk->capacity + 1]) {
        STACKDUMP(open_log_file, stk, kWrongCanaryRight);
        return kWrongCanaryRight;
    }

    if (!check_data_hash(stk)) {
        STACKDUMP(open_log_file, stk, kHashMismatch);
        return kHashMismatch;
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
        case kErrorOpening: return "Open open_log_file failed";
        case kErrorClosing: return "Close open_log_file failed";
        case kWrongCanaryLeft: return "Wrong canary left value";
        case kWrongCanaryRight: return "Wrong canary right value";
        case kHashMismatch: return "Hash mismatch. Stack corrupted";
        case kNoCommand: return "No command found";
        default: return "Unknown error";
    }
}

double bin_search(Stack_t n) {
    double l = 0;
    double r = (double)n;

    while (r - l > 1e-9) {
        
        double mid = (l + r) / 2;
        if (mid * mid  > n) {
            r = mid;
        } else {
            l = mid;
        }
    }

    return l;
} // убрать куда-нибудь