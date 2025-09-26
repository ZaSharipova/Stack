#include "StackHash.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

uint32_t hash_data(const Stack_t *data, ssize_t size) {
    assert(data);

    uint32_t hash = 5381;
    for (ssize_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + (uint32_t)data[i];
    }
    return hash;
}

void update_data_hash(Stack_Info *stk) {
    assert(stk);

    stk->data_hash = hash_data(stk->data, stk->size);
}

int check_data_hash(const Stack_Info *stk) {
    assert(stk);

    return (stk->data_hash == hash_data(stk->data, stk->size));
}


