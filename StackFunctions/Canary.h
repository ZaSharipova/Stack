#ifndef CANARY_H_
#define CANARY_H_

#include <time.h>

#include "StackFunctions.h"
#include "StructsEnums.h"

StackErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file);
uint32_t Init_Canary(void);

#endif //CANARY_H_