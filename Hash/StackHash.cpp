#include "StackHash.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"

uint32_t Hash_Data(const Stack_t *data, ssize_t size) {
    assert(data);

    uint32_t hash = 5381;
    for (ssize_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + (uint32_t)data[i];
    }
    return hash;
}

void Update_Data_Hash(Stack_Info *stk) {
    assert(stk);

#ifdef _DEBUG
    stk->data_hash = Hash_Data(stk->data, stk->size);
#endif
}

int Check_Data_Hash(const Stack_Info *stk) {
    assert(stk);
    
#ifdef _DEBUG
    return (stk->data_hash == Hash_Data(stk->data, stk->size));
#endif
}


