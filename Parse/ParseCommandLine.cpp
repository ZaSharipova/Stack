#include "ParseCommandLine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "StructsEnums.h"
#include "FileOperations.h"

ParseErr_t Parse_Input(const char **argv, int argc, Files *in_out_files) {
    assert(argv);

    int pos = 1;

    if (argc == 2) {
        fprintf(stderr, "Please, enter normally next time.\n");
        return kErrorParsing;
    }

    if (argc > 2) {
        while (pos + 1 < argc) {
            if (strcmp(argv[pos], LOG_MODE) == 0) {
                in_out_files->log_file = argv[pos + 1];
                pos += 2;

            } else if (strcmp(argv[pos], IN_MODE) == 0) {
                in_out_files->in_file = argv[pos + 1];
                pos += 2;

            } else if (strcmp(argv[pos], OUT_MODE) == 0) {
                in_out_files->out_file = argv[pos + 1];
                pos += 2;

            } else {
                fprintf(stderr, "Wrong mode %s.\n", argv[pos]);
                return kWrongMode;
            }
        }
    } 

    return kNoError;
}