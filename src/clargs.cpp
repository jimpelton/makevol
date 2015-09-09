//
// Created by jim on 6/12/15.
//

#include "clargs.h"
#include <tclap/CmdLine.h>

int parseThem(int argc, const char *argv[], CommandLineOpts &opts)
try {
    TCLAP::CmdLine cmd("Make data, save kittens!", ' ');

    std::string const defaultVolName = "outvolume.raw";
    std::string const defaultDatName = "outvolume.dat";


    // volume file name
    TCLAP::ValueArg<std::string>
            outVolArg("f", "vol-file", "Output data file", false,
                      defaultVolName, "string");
    cmd.add(outVolArg);


    // dat file name
    TCLAP::ValueArg<std::string>
            outDatArg("d", "dat-file", "Output .dat file (descriptor file)",
                      false, defaultDatName, "string");
    cmd.add(outDatArg);


    // volume shape
    TCLAP::ValueArg<std::string>
            volumeShapeArg("s", "shape", "Shape of the volume", false,
                           "none", "string");
    cmd.add(volumeShapeArg);


    // volume dims
    TCLAP::ValueArg<size_t> xdimArg("", "volx", "Volume x dim.", false, 1, "uint");
    cmd.add(xdimArg);

    TCLAP::ValueArg<size_t> ydimArg("", "voly", "Volume y dim.", false, 1, "uint");
    cmd.add(ydimArg);

    TCLAP::ValueArg<size_t> zdimArg("", "volz", "Volume z dim.", false, 1, "uint");
    cmd.add(zdimArg);


    // boarder dims
    TCLAP::ValueArg<size_t> xBorderArg("", "bx", "Border x width.", false, 1, "uint");
    cmd.add(xBorderArg);

    TCLAP::ValueArg<size_t> yBorderArg("", "by", "Border y height.", false, 1, "uint");
    cmd.add(yBorderArg);

    TCLAP::ValueArg<size_t> zBorderArg("", "bz", "Border z depth.", false, 1, "uint");
    cmd.add(zBorderArg);


    // min, max values
    TCLAP::ValueArg<float>
            minValArg("", "min", "Min generated value.", false, 0.0f, "float");
    cmd.add(minValArg);

    TCLAP::ValueArg<float>
            maxValArg("", "max", "Max generated value.", false, 1.0f, "float");
    cmd.add(maxValArg);

    // parse args //
    cmd.parse(argc, argv);

    opts.outVolFile = outVolArg.getValue();
    opts.outDatFile = outDatArg.getValue();

    opts.volShape = volumeShapeArg.getValue();

    opts.volXDim = xdimArg.getValue();
    opts.volYDim = ydimArg.getValue();
    opts.volZDim = zdimArg.getValue();

    opts.volBorderX = xBorderArg.getValue();
    opts.volBorderY = yBorderArg.getValue();
    opts.volBorderZ = zBorderArg.getValue();

    opts.minval = minValArg.getValue();
    opts.maxval = maxValArg.getValue();


    return argc;

} catch (TCLAP::ArgException &e) {
    std::cout << "Error parsing command line args: " << e.error() <<
    " for argument " << e.argId() << std::endl;
    return 0;
}

void printThem(std::ostream &os, CommandLineOpts const &opts)
{
    os << "Out file: " << opts.outVolFile << '\n'
    << "Out dat file: " << opts.outDatFile << '\n'
    << "Volume shape: " << opts.volShape << '\n'
    << "Volume dimensions: " << opts.volXDim << ", " << opts.volYDim << ", " << opts.volZDim << '\n'
    << "Volume boarder widths: " << opts.volBorderX << ", " << opts.volBorderY << ", " << opts.volBorderZ << '\n'
    << "Max, Min values: " << opts.maxval << ", " << opts.minval << '\n';
}


