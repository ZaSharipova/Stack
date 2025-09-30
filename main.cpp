#include <stdio.h>
#include <unistd.h>

#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "ParseInput.h"
#include "StackFunctions.h"
#include "StructsEnums.h"

int main(int argc, const char **argv) {
    Files in_out_files = {NULL, NULL, NULL, NULL, NULL, NULL};

    ParseErr_t read_write_error = kNoError;
    StackErr_t err = kSuccess;

    CALL_CHECK_IN_OUT_RETURN(Parse_Input(argv, argc, &in_out_files));

    CALL_CHECK_IN_OUT_RETURN(Handle_Open_File(&in_out_files));

    INIT_STACK(stk1);
    #ifdef _CANARY 
    stk1.canary_left = 0, 
    stk1.canary_right = 0, 
    stk1.real_data = NULL;
#endif

#ifdef _HASH
    stk1.data_hash = 0;
#endif

    CHECK_STACK_RETURN(StackCtor(&stk1, 1, in_out_files.open_log_file));

    CHECK_STACK_RETURN(Parse_Graphics(&stk1, in_out_files.open_in_file, in_out_files.open_log_file, in_out_files.open_out_file));

    CHECK_STACK_RETURN(StackDtor(&stk1, in_out_files.open_log_file));

    CALL_CHECK_IN_OUT_RETURN(Handle_Close_File(in_out_files));

    return 0;
}