#include "ParseInput.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

static Stack_t Add(Stack_t a, Stack_t b);
static Stack_t Sub(Stack_t a, Stack_t b) ;
static Stack_t Mul(Stack_t a, Stack_t b);
static StackErr_t StackBinaryOp(Stack_Info *stk, Stack_t (*op)(Stack_t, Stack_t), FILE *file);

StackErr_t Parse_Graphics(Stack_Info *stk, FILE *file) {
    assert(stk);

    char *command = (char *)calloc(1000, sizeof(char));
    if (command == NULL) {
        printf("No memory in calloc to read buf.\n");
        return kNoMemory;
    }

    fscanf(file, "%s", command);

    StackErr_t err = kSuccess;
    Stack_t num = 0;


    while (strcmp(command, HLT) != 0) {

        if (strcmp(command, PUSH) == 0) {
            if (fscanf(file, " " MODE " ", &num) != 1) {
                free(command);
                return kErrorSize; //
            }
            CALL_CHECK(StackPush(stk, num, file));

        } else if (strcmp(command, POP) == 0) {
            CALL_CHECK(StackPop(stk, &num, file));

        } else if (strcmp(command, ADD) == 0) {
            CALL_CHECK(StackBinaryOp(stk, Add, file));

        } else if (strcmp(command, SUB) == 0) {
            CALL_CHECK(StackBinaryOp(stk, Sub, file));

        } else if (strcmp(command, MUL) == 0) {
            CALL_CHECK(StackBinaryOp(stk, Mul, file));

        } else if (strcmp(command, DIV) == 0) {
            Stack_t rhs = 0, lhs = 0;
            CALL_CHECK(StackPop(stk, &rhs, file));
            CALL_CHECK(StackPop(stk, &lhs, file));

            if (rhs != 0) {
                CALL_CHECK(StackPush(stk, lhs / rhs, file));
            } else {
                printf("Zero Div error.\n");
                free(command);
             // код ошибки деления на 0 должен быть определён
            }

        } else if (strcmp(command, SQRT) == 0) {
            CALL_CHECK(StackPop(stk, &num, file));
            num = bin_search((Stack_t)num);
            CALL_CHECK(StackPush(stk, num, file));

        } else {
            fprintf(file, "No command found.\n");
            free(command);
            return kNoCommand;
        }

        fscanf(file, "%s", command);
    }

    free(command);
    return kSuccess;
}

static Stack_t Add(Stack_t a, Stack_t b) { 
    return a + b; 
}

static Stack_t Sub(Stack_t a, Stack_t b) { 
    return a - b; 

}

static Stack_t Mul(Stack_t a, Stack_t b) { 
    return a * b; 
}

static StackErr_t StackBinaryOp(Stack_Info *stk, Stack_t (*op)(Stack_t, Stack_t), FILE *file) {
    assert(stk);
    assert(op);
    assert(file);
    
    Stack_t rhs = 0, lhs = 0, result = 0;
    StackErr_t err = kSuccess;

    err = StackPop(stk, &rhs, file);
    if (err != kSuccess) return err;

    err = StackPop(stk, &lhs, file);
    if (err != kSuccess) return err;

    result = op(lhs, rhs);

    err = StackPush(stk, result, file);
    return err;
}