

#include "bbox.h"
#include "sphericalvolume.h"
#include "cuboidvolume.h"

#include <tclap/CmdLine.h>

#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>


void writeDatFile(const std::string &prefix, 
    size_t resx, size_t resy, size_t resz,
    unsigned int thickX, unsigned int thickY, unsigned int thickZ,
    const std::string &format, const std::string &path = "") 
{
    std::string fname(prefix + ".raw");
    std::string datFilePath(prefix + ".dat");
    std::stringstream ss;
    ss << "ObjectFileName : " << fname << "\n"
        "Resolution : " << resx << " " << resy << " " << resz << "\n"
        "SliceThickness : " << thickX << " " << thickY << " " << thickZ << "\n"
        "Format : " << format << "\n"
        "ObjectModel : I\n";

    std::ofstream file(datFilePath);
    if (!file.is_open()) {
        std::cerr << "Could not open dat file for writing: " << datFilePath;
        return;
    }

    file << ss.str();
    file.flush();
    file.close();
}

void usage(const std::string &msg) 
{
    if (!msg.empty())
        std::cout << msg << "\n";

    std::cout << "usage: width height depth [minval maxval] [bx by bz]\n"
        "\t bx, by, bz define an empty border region of 0.0f values." << std::endl;
}

void usage() 
{
    usage("");
}


enum class VolumeType
{
    Cube,
    Sphere,
    File,
    MAX
};

struct CommandLineOpts
{
    size_t volXDim;
    size_t volYDim;
    size_t volZDim;

    size_t volBorderX;
    size_t volBorderY;
    size_t volBorderZ;

    float minval;
    float maxval;

    std::string outVolFile;
    std::string outDatFile;

};

int parseThem(int argc, const char *argv[], CommandLineOpts &opts)
try
{
    TCLAP::CmdLine cmd("Make 3D data in your living room!", ' ');

    const std::string defaultVolName = "outvolume.raw";
    const std::string defaultDatName = "outvolume.dat";

    TCLAP::ValueArg<std::string> outVolFileArg("f", "vol-file", "Output data file", false, defaultVolName, "string");
    cmd.add(outVolFileArg);

    TCLAP::ValueArg<std::string> outDatfileArg("f", "dat-file", "Output .dat file (descriptor file)", false, defaultDatName, "string");
    cmd.add(outDatfileArg);

    // volume dims
    TCLAP::ValueArg<size_t> xdimArg("", "volx", "Volume x dim.", false, 1, "uint");
    cmd.add(xdimArg);

    TCLAP::ValueArg<size_t> ydimArg("", "voly", "Volume y dim.", false, 1, "uint");
    cmd.add(ydimArg);

    TCLAP::ValueArg<size_t> zdimArg("", "volz", "Volume z dim.", false, 1, "uint");
    cmd.add(zdimArg);

    // volume dims
    TCLAP::ValueArg<size_t> xBorderArg("", "bx", "Border x width.", false, 1, "uint");
    cmd.add(xBorderArg);

    TCLAP::ValueArg<size_t> yBorderArg("", "by", "Border y height.", false, 1, "uint");
    cmd.add(yBorderArg);

    TCLAP::ValueArg<size_t> zBorderArg("", "bz", "Border z depth.", false, 1, "uint");
    cmd.add(zBorderArg);

    TCLAP::ValueArg<float> minValArg("", "min", "Min generated value.", false, 0.0f, "float");
    cmd.add(zBorderArg);

    TCLAP::ValueArg<float> maxValArg("", "max", "Max generated value.", false, 1.0f, "float");
    cmd.add(zBorderArg);

    TCLAP::ValueArg<std::string> volumeTypeArg("s", "shape", "Shape of the volume", false, "sphere", "string");

    cmd.parse(argc, argv);

    opts.volXDim = xdimArg.getValue();
    opts.volYDim = ydimArg.getValue();
    opts.volZDim = zdimArg.getValue();

    opts.volBorderX= xBorderArg.getValue();
    opts.volBorderY= yBorderArg.getValue();
    opts.volBorderZ= zBorderArg.getValue();

    opts.minval = minValArg.getValue();
    opts.maxval = minValArg.getValue();

    opts.outVolFile = outVolFileArg.getValue();
    opts.outDatFile = outDatfileArg.getValue();

    return argc;

} catch (TCLAP::ArgException &e) {
    std::cout << "Error parsing command line args: " << e.error() <<
        " for argument " << e.argId() << std::endl;
    return 0;
}

int main (int argc, char* argv[]) 
{
    
    if (argc < 4) {
        usage("Not enough arguments.");
        return 1;
    }

    unsigned int width = atoi(argv[1]);
    unsigned int height = atoi(argv[2]);
    unsigned int depth = atoi(argv[3]);
    
    float minval = 0;
    if (argc > 4) {
        minval = atof(argv[4]);
    }

    float maxval = std::numeric_limits<float>::max();
    if (argc > 5) {
        maxval = atof(argv[5]);
    }

    unsigned int bX = 0;
    if (argc > 6) {
        bX = atoi(argv[6]);
    }

    unsigned int bY = 0;
    if (argc > 7) {
        bY = atoi(argv[7]);
    }

    unsigned int bZ = 0;
    if (argc > 8) {
        bZ = atoi(argv[8]);
    }

    float range = maxval - minval;

    if (range < 0.0f) {
        usage("Maxval must be larger than minval.");
        return 1;
    }

    BBox vol({ 0, 0, 0 }, {width,height,depth});
    BBox inner({ bX, bY, bZ }, {width-bX, height-bY, depth-bZ});
    
    CuboidVolume cube(vol, inner, minval, maxval, "outfile_cube.raw");
    cube.generate();
    writeDatFile("outfile_cube", width, height, depth, 1, 1, 1, "FLOAT");

    SphericalVolume sph(vol, inner, minval, maxval, "outfile_sphere.raw");
    sph.generate();
    writeDatFile("outfile_sphere", width, height, depth, 1, 1, 1, "FLOAT");


    std::cout << "El fin! (supposed success!)." << std::endl;
    return 0;
}

