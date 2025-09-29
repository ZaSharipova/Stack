#ifndef STACK_HASH_H_
#define STACK_HASH_H_

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

uint32_t hash_data(const Stack_t *data, ssize_t size);
void update_data_hash(Stack_Info *stk);
int check_data_hash(const Stack_Info *stk);

#endif //STACK_HASH_H_