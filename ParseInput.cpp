#include "ParseInput.h"

#include <stdio.h>
#include <string.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

#define MODE "%d"
#define PUSH "PUSH"
#define ADD "ADD"
#define POP "POP"
#define SUB "SUB"
#define MUL "MUL"
#define DIV "DIV"
#define SQRT "SQRT"
#define HLT "HLT"

#include "StackFunctions.h"

StackErr_t parse_graphics(Stack_Info *stk, FILE *file) {
    assert(stk);

    char command[1000];
    fscanf(file, "%s", command);

    Stack_t num = 0;
    StackErr_t err = kSuccess;
    while (strcmp(command, HLT) != 0) {
        printf("%s\n", command);
        if (strcmp(command, PUSH) == 0) {
            fscanf(file, " " MODE " ", &num);
            CALL_CHECK(StackPush(stk, num, file));

        } else if (strcmp(command, POP) == 0) {
            CALL_CHECK(StackPop(stk, &num, file));

        } else if (strcmp(command, ADD) == 0) {
            Stack_t result = 0;
            CALL_CHECK(StackPop(stk, &num, file));
            result += num;
            CALL_CHECK(StackPop(stk, &num, file));
            result += num;
            CALL_CHECK(StackPush(stk, result, file));

        } else if (strcmp(command, SUB) == 0) {
            Stack_t result = 0;
            CALL_CHECK(StackPop(stk, &num, file));
            result = num;
            CALL_CHECK(StackPop(stk, &num, file));
            result -= num;
            CALL_CHECK(StackPush(stk, result, file));

        } else if (strcmp(command, MUL) == 0) {
            Stack_t result = 0;
            CALL_CHECK(StackPop(stk, &num, file));
            result = num;
            CALL_CHECK(StackPop(stk, &num, file));
            result *= num;
            CALL_CHECK(StackPush(stk, result, file));

        } else if (strcmp(command, DIV) == 0) {
            Stack_t result = 0;
            CALL_CHECK(StackPop(stk, &num, file));
            result = num;
            CALL_CHECK(StackPop(stk, &num, file));
            result /= num;
            CALL_CHECK(StackPush(stk, result, file)); //проверка на 0

        } else if (strcmp(command, POP) == 0) {
            CALL_CHECK(StackPop(stk, &num, file));

        } else if (strcmp(command, SQRT) == 0) {
            CALL_CHECK(StackPop(stk, &num, file));
            num = bin_search(num);
            CALL_CHECK(StackPush(stk, num, file));

        } else {
            fprintf(file, "No command found\n");
            return kNoCommand;
        }
        fscanf(file, "%s", command);
    }

    return kSuccess;
}