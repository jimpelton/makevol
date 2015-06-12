//
// Created by jim on 6/12/15.
//

#ifndef clargs_h__
#define clargs_h__

#include <string>

struct CommandLineOpts {
    std::string outVolFile;
    std::string outDatFile;

    std::string volShape;

    size_t volXDim;
    size_t volYDim;
    size_t volZDim;

    size_t volBorderX;
    size_t volBorderY;
    size_t volBorderZ;

    float minval;
    float maxval;
};

int parseThem(int argc, const char *argv[], CommandLineOpts &opts);

#endif // !clargs_h__
