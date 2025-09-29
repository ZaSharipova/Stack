#ifndef PARSE_COMMAND_LINE_H_
#define PARSE_COMMAND_LINE_H_

#include "StructsEnums.h"

#define IN_MODE "--in"
#define OUT_MODE "--out"
#define LOG_MODE "--log"

ParseErr_t Parse_Input(const char **argv, int argc, Files *in_out_files);

#endif //PARSE_COMMAND_LINE_H_