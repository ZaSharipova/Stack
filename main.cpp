#include <stdio.h>
#include <unistd.h>

#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "ParseInput.h"
#include "StackFunctions.h"
#include "StructsEnums.h"
#include "OperateLogFile.h"

int main(int argc, const char **argv) {
    Files in_out_files = {NULL, NULL, NULL, NULL};

    ParseErr_t read_write_error = kNoError;
    int err = 0;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));

    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    INIT_STACK(stk1);

    CHECK_STACK_RETURN((int)StackCtor(&stk1, 1, in_out_files.open_log_file));

    CHECK_STACK_RETURN((int)ParseGraphics(&stk1, in_out_files.open_in_file, in_out_files.open_log_file, in_out_files.open_out_file));

    CHECK_STACK_RETURN((int)StackDtor(&stk1, in_out_files.open_log_file));

    CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(in_out_files));

    return 0;
}