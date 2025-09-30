#ifndef STACK_FUNCTIONS_H_
#define STACK_FUNCTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "StructsEnums.h"

#define RED_COLOR "\033[1;31m"
#define RESET_COLOR "\033[0m"
#define RED(stream)   (isatty(fileno(stream)) ? RED_COLOR : "")
#define RESET(stream) (isatty(fileno(stream)) ? RESET_COLOR : "")

#define INCREASE_VALUE 2
#define DECREASE_VALUE 4

#define STACKDUMP(open_log_file, stk_name, err) StackDump(open_log_file, *stk_name, __func__, __LINE__, __FILE__, err, #stk_name)

StackErr_t StackCtor(Stack_Info *stk, ssize_t value, FILE *open_log_file);
StackErr_t StackPush(Stack_Info *stk, Stack_t value, FILE *open_log_file);
StackErr_t StackPop(Stack_Info *stk, Stack_t *value, FILE *open_log_file);
Realloc_Mode CheckSize(ssize_t size, ssize_t *capacity);
StackErr_t StackRealloc(Stack_Info *stk, FILE *open_log_file);
StackErr_t StackDtor(Stack_Info *stk, FILE *open_log_file);
Stack_t StackDump(FILE *open_log_file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *stk_name);
StackErr_t CheckError(Stack_Info *stk, FILE *open_log_file);
const char* GetErrorString(StackErr_t err);

uint32_t Init_Canary(void);
StackErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file);

double bin_search(Stack_t n);

#endif //STACK_FUNCTIONS_H_