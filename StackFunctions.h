#ifndef STACK_FUNCTIONS_H_
#define STACK_FUNCTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "StructsEnums.h"

#define RED(stream)   (isatty(fileno(stream)) ? "\033[1;31m" : "")
#define RESET(stream) (isatty(fileno(stream)) ? "\033[0m" : "")

#define INCREASE_VALUE 2
#define DECREASE_VALUE 4

StackErr_t StackCtor(Stack_Info *stk, ssize_t value, FILE *file);
StackErr_t StackPush(Stack_Info *stk, Stack_t value, FILE *file);
StackErr_t StackPop(Stack_Info *stk, Stack_t *value, FILE *file);
Realloc_Mode CheckSize(ssize_t size, ssize_t *capacity);
StackErr_t StackRealloc(Stack_Info *stk, FILE *file);
StackErr_t StackDtor(Stack_Info *stk, FILE *file);
Stack_t StackDump(FILE *file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *name);
StackErr_t CheckError(Stack_Info *stk, FILE *file);
const char* GetErrorString(StackErr_t err);

uint32_t Init_Canary(void);
StackErr_t MakeCanary(Stack_Info *stk, FILE *file);

double bin_search(Stack_t n);
#endif //STACK_FUNCTIONS_H_