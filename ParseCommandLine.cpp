#include "ParseCommandLine.h"

#include <assert.h>
#include <stdio.h>

ParseErr_t parse_input(const char **argv, int argc, const char **filename) {
    assert(argv);

    switch(argc) {
    case (1):
        return kNoNeedToOpen;
    case (2):
        *filename = argv[1];
        return kOpen;
    default:
       return kNoType; 
    }

}