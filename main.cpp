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

    ParseErr_t read_write_error = kNoError;
    StackErr_t err = kSuccess;

    CALL_CHECK_IN_OUT(Parse_Input(argv, argc, &in_out_files));

    CALL_CHECK_IN_OUT(Handle_Open_File(&in_out_files));

#ifdef _DEBUG
    INIT_STACK(stk1);
#else
    INIT_STACK(stk1);
#endif

    CALL_CHECK_STACK(StackCtor(&stk1, 3, in_out_files.open_log_file));

    CALL_CHECK_STACK(Parse_Graphics(&stk1, in_out_files.open_in_file, in_out_files.open_log_file))

    CALL_CHECK_STACK(StackDtor(&stk1, in_out_files.open_log_file));

    CALL_CHECK_IN_OUT(Handle_Close_File(in_out_files));

    return 0;
}