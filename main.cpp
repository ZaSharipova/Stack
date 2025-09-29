#include <stdio.h>
#include <unistd.h>

#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "ParseInput.h"
#include "StackFunctions.h"
#include "StructsEnums.h"

#ifndef _DEBUG
    #define INIT_STACK(name) \
        Stack_Info name = {.data = nullptr, .size = 0, .capacity = 0}
#else
    #define INIT_STACK(name) \
        Stack_Info name = {.data = nullptr, .size = 0, .capacity = 0, \
            .create_var_info.file_name = __FILE__, .create_var_info.func_name = __func__, \
            .create_var_info.var = #name, .create_var_info.line = __LINE__ }
#endif

int main(int argc, const char **argv) {
    Files in_out_files = {.in_file = NULL, .out_file = NULL};

    ParseErr_t error = kNoError;
    StackErr_t err = kSuccess;

    error = Parse_Input(argv, argc, &in_out_files);
    if (error != kNoError) {
        return error;
    }

    err = Handle_Open_File(&in_out_files);
    if (err != kSuccess) {
        //TODO обработку ошибок
        return err;
    }

#ifdef _DEBUG
    INIT_STACK(stk1);
#else
    INIT_STACK(stk1);
#endif

    CALL_CHECK(StackCtor(&stk1, 1, in_out_files.open_log_file));

    err = Parse_Graphics(&stk1, in_out_files.open_in_file);
    if (err != kSuccess) {
        printf("sdffd");
        return err;
    }

    CALL_CHECK(StackDtor(&stk1, in_out_files.open_log_file));

    err = Handle_Close_File(in_out_files);
    if (err != kSuccess) {
        return err;
    }

    return 0;
}