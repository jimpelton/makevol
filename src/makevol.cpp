#include "bbox.h"
#include "sphericalvolume.h"
//#include "cuboidvolume.h"
#include "point3.h"
#include "clargs.h"

#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>

using P3f = Point3<float>;
using BBfloat = BBox<float>;

enum class VolumeType {
    Cube,
    Sphere,
    File,
    MAX
};


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
void usage(const std::string &msg) {
    if (!msg.empty()) {
        std::cout << msg << "\n";
    }

    std::cout << "Use -h for command line arguments." << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
void usage() {
    usage("");
}


///////////////////////////////////////////////////////////////////////////////
template<typename T>
size_t makeVol(CommandLineOpts &opts, const BBox<T> &vol, const BBox<T> &inner)
{
//    if (opts.volShape == "cube") {
//        CuboidVolume cube(vol, inner, opts.minval, opts.maxval, opts.outVolFile);
//        cube.generate(1024);
//    } else
    if (opts.volShape == "sphere") {
        SphericalVolume<T> sph(vol, inner, opts.minval, opts.maxval);
        sph.outfile(opts.outVolFile);
        return sph.generate(1024);
    } else {
        std::cout << "Not a valid volume shape." << std::endl;
        return 0;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
int main(int argc, const char *argv[]) {
    CommandLineOpts opts;
    parseThem(argc, argv, opts);
    printThem(std::cout, opts);

    float range{ opts.maxval - opts.minval };

    if (range <= 0.0f) {
        usage("Maxval must be larger than minval.");
        return 1;
    }

    BBox<size_t> vol{ Point3<size_t>(0, 0, 0), Point3<size_t>(opts.volXDim, opts.volYDim, opts.volZDim) };
    BBox<size_t> inner{
        Point3<size_t>(opts.volBorderX, opts.volBorderY, opts.volBorderZ),
        Point3<size_t>(opts.volXDim - opts.volBorderX,
            opts.volYDim - opts.volBorderY,
            opts.volZDim - opts.volBorderZ ) };

    if (! makeVol<float>(opts, vol, inner)) {
        exit(1);
    }

    writeDatFile(opts.outDatFile, opts.volXDim, opts.volYDim, opts.volZDim, 1, 1, 1, "FLOAT");

    std::cout << "El fin! (supposed success!)." << std::endl;
    return 0;
};

