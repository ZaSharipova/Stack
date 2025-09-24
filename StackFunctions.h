#ifndef STACK_FUNCTIONS_H_
#define STACK_FUNCTIONS_H_

#include "StructsEnums.h"

StackErr_t StackCtor(Stack_Info *stk, Stack_t value);
StackErr_t StackPush(Stack_Info *stk, Stack_t value);
StackErr_t StackPop(Stack_Info *stk, Stack_t *value);
StackErr_t StackRealloc(Stack_Info *stk, char mode);
StackErr_t StackDtor(Stack_Info *stk);
Stack_t StackDump(FILE *file, Stack_Info stk, const char *func_name, int line, const char *file_from, StackErr_t err, const char *name);
StackErr_t CheckError(Stack_Info *stk);

#endif //STACK_FUNCTIONS_H_