#include "bbox.h"
#include "sphericalvolume.h"
#include "cuboidvolume.h"
#include "point3.h"
#include "clargs.h"

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

void usage(const std::string &msg) {
    if (!msg.empty()) {
        std::cout << msg << "\n";
    }

    std::cout << "usage: width height depth [minval maxval] [bx by bz]\n"
            "\t bx, by, bz define an empty border region of 0.0f values." <<
    std::endl;
}

void usage() {
    usage("");
}


enum class VolumeType {
    Cube,
    Sphere,
    File,
    MAX
};


int main(int argc, const char *argv[]) {
    CommandLineOpts opts;
    if (parseThem(argc, argv, opts) == 0) {
        usage();
        return 1;
    }


    float range{ opts.maxval - opts.minval };

    if (range <= 0.0f) {
        usage("Maxval must be larger than minval.");
        return 1;
    }

    BBox vol{ Point3(0, 0, 0), Point3(opts.volXDim, opts.volYDim, opts.volZDim) };

    BBox inner{ Point3(opts.volBorderX,
                      opts.volBorderY,
                      opts.volBorderZ),
               Point3(opts.volXDim - opts.volBorderX,
                      opts.volYDim - opts.volBorderY,
                      opts.volZDim - opts.volBorderZ
               ) };

    if (opts.volShape == "cube") {

        CuboidVolume cube(vol, inner, opts.minval, opts.maxval,
                          opts.outVolFile);
        cube.generate();
        writeDatFile(opts.outDatFile,
                     opts.volXDim, opts.volYDim, opts.volZDim,
                     1, 1, 1,
                     "FLOAT");
    }
    else if (opts.volShape == "sphere") {
        SphericalVolume sph(vol, inner, opts.minval, opts.maxval, opts.outVolFile);
        sph.generate();
        writeDatFile(opts.outDatFile,
                     opts.volXDim, opts.volYDim, opts.volZDim,
                     1, 1, 1,
                     "FLOAT");
    }
    else {
        std::cout << "Not a valid volume shape." << std::endl;
        exit(1);
    }

    std::cout << "El fin! (supposed success!)." << std::endl;
    return 0;
};

