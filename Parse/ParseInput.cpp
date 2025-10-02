#include "ParseInput.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "StructsEnums.h"

static Stack_t Add(Stack_t a, Stack_t b);
static Stack_t Sub(Stack_t a, Stack_t b);
static Stack_t Mul(Stack_t a, Stack_t b);
static StackErr_t StackBinaryOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *file);

StackErr_t ParseGraphics(Stack_Info *stk, FILE *file, FILE *open_log_file, FILE *open_out_file) {
    assert(stk);
    assert(file);
    assert(open_out_file);

    if (file == stdin) {
        printf("Введите команды PUSH, ADD, POP, DIV, MUL, SQRT.\n");
    }

    char *command = (char *) calloc (100, sizeof(char));
    if (command == NULL) {
        printf("No memory in calloc to read buf.\n");
        return kNoMemory;
    }

    fscanf(file, "%s", command);

    StackErr_t err = kSuccess;
    Stack_t num = 0;

    while (strcmp(command, HLT) != 0) {

        if (strcmp(command, PUSH) == 0) {
            if (fscanf(file, "" MY_SPEC "", &num) != 1) {
                free(command);
                return kNumberNotWritten;
            }
            CHECK_STACK_FREE_RETURN(StackPush(stk, num, open_log_file));

        } else if (strcmp(command, POP) == 0) {
            CHECK_STACK_FREE_RETURN(StackPop(stk, &num, open_log_file));

        } else if (strcmp(command, ADD) == 0) {
            CHECK_STACK_FREE_RETURN(StackBinaryOperation(stk, Add, open_log_file));

        } else if (strcmp(command, SUB) == 0) {
            CHECK_STACK_FREE_RETURN(StackBinaryOperation(stk, Sub, open_log_file));

        } else if (strcmp(command, MUL) == 0) {
            CHECK_STACK_FREE_RETURN(StackBinaryOperation(stk, Mul, open_log_file));

        } else if (strcmp(command, DIV) == 0) {
            Stack_t second = 0, first = 0;
            CHECK_STACK_FREE_RETURN(StackPop(stk, &second, open_log_file));
            CHECK_STACK_FREE_RETURN(StackPop(stk, &first, open_log_file));

            if (second != 0) {
                CHECK_STACK_FREE_RETURN(StackPush(stk, first / second, open_log_file));
            } else {
                printf("Zero Div error.\n");
                free(command);
                return kZeroNumber;
            }

        } else if (strcmp(command, SQRT) == 0) {
            CHECK_STACK_FREE_RETURN(StackPop(stk, &num, open_log_file));
            num = (Stack_t)Sqrt(num);
            CHECK_STACK_FREE_RETURN(StackPush(stk, num, open_log_file));

        } else if (strcmp(command, OUT) == 0) {
            CHECK_STACK_FREE_RETURN(StackPop(stk, &num, open_log_file));
            fprintf(open_out_file, "" MY_SPEC "\n", num);

        } else {
            fprintf(open_log_file, "No command found.\n");
            free(command);
            return kNoCommandFound;
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

static StackErr_t StackBinaryOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *file) {
    assert(stk);
    assert(operation);
    assert(file);

    Stack_t second = 0, first = 0, result = 0;
    //StackErr_t error = kSuccess;
    StackErr_t err = kSuccess;

    CHECK_ERROR_RETURN(StackPop(stk, &second, file));
    CHECK_ERROR_RETURN(StackPop(stk, &first, file));

    result = operation(first, second);

    err = StackPush(stk, result, file);
    return err;
}