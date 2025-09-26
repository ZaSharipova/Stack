#ifndef PARSE_COMMAND_LINE_H_
#define PARSE_COMMAND_LINE_H_

#include "StructsEnums.h"

ParseErr_t parse_input(const char **argv, int argc, const char **filename);

#endif //PARSE_COMMAND_LINE_H_