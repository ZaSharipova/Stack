#ifndef STACK_HASH_H_
#define STACK_HASH_H_

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

uint32_t Hash_Data(const Stack_t *data, ssize_t size);
void Update_Data_Hash(Stack_Info *stk);
int Check_Data_Hash(const Stack_Info *stk);

#endif //STACK_HASH_H_