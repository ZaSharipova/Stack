#include <stdio.h>
#include <unistd.h>

#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "StackFunctions.h"
#include "StructsEnums.h"

#define RED(stream)   (isatty(fileno(stream)) ? "\033[1;31m" : "")
#define RESET(stream) (isatty(fileno(stream)) ? "\033[0m" : "")

// #define DEBUG
#define INIT_DEBUG(name) \
    Stack_Info name = {.data = nullptr, .size = 0, .capacity = 0, \
        .create_var_info.file_name = __FILE__, .create_var_info.func_name = __func__, \
        .create_var_info.var = #name, .create_var_info.line = __LINE__ } \


#define INIT_NDEBUG(name) \
    Stack_Info name = {.data = nullptr, .size = 0, .capacity = 0}

#define CALL_CHECK(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }

int main(int argc, const char **argv) {
    const char *log_file = NULL;
    ParseErr_t error = parse_input(argv, argc, &log_file);
    if (error == kNoType) {
        return error;
    }

    FILE *open_log = NULL;
    if (error == kOpen) {
        open_log = Open_File(log_file, "w");
        if (open_log == NULL) {
            return kErrorOpening;
        }
        fprintf(open_log, "%s hello %s", RED(open_log), RESET(open_log));
        fclose(open_log);
    }


#ifdef _DEBUG
    INIT_DEBUG(stk1);
#else
    INIT_NDEBUG(stk1);
#endif


    StackErr_t err = kSuccess;
    CALL_CHECK(StackCtor(&stk1, 1, open_log));
    CALL_CHECK(StackPush(&stk1, 10, open_log));  
    CALL_CHECK(StackPush(&stk1, 20, open_log));
    CALL_CHECK(StackPush(&stk1, 30, open_log));  

    Stack_t x = 0; 
    CALL_CHECK(StackPop(&stk1, &x, open_log));   
    CALL_CHECK(StackDtor(&stk1, open_log));

    // err = StackCtor(&stk1, 10); //change
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 10);
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 20);
    // CHECK_ERROR(err);

    // err = StackPush(&stk1, 30);
    // CHECK_ERROR(err);

    // Stack_t x = 0;
    // err = StackPop(&stk1, &x);
    // CHECK_ERROR(err);

    // err = StackDtor(&stk1);
    // CHECK_ERROR(err);

    //StackDump(stdout, stk1);
    return 0;
}