
#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "point3.h"

class BBox {
public:
    BBox() { }
    
    BBox(const Point3 &min, const Point3 &max) 
        : m_minp(min), m_maxp(max) 
    { }
    
    BBox(const BBox &o) 
        : m_minp(o.min()), m_maxp(o.max()) 
    { }

    ~BBox() { }

public:

    void shrink(const BBox &diff) {
        m_minp += diff.m_minp;
        m_maxp -= diff.m_maxp;
    }

    Point3 dims() {
        return Point3(m_maxp-m_minp);
    }


    inline size_t num_vox() const { 
        Point3 diff(m_maxp-m_minp);
        return diff.x() * diff.y() * diff.z();
    }
   
    inline bool contains(const Point3 &p) { return p < m_maxp && p > m_minp; }

    inline const Point3& min() const { return m_minp; }
    inline const Point3& max() const { return m_maxp; }

private:
    Point3 m_minp, m_maxp;

};

class Volume {
public:
    Volume(const BBox &ext, const BBox &dataExt, float minval, float maxval, const std::string &outfilePath)
        : m_outfilePath(outfilePath)
        , m_ext(ext)
        , m_dataExt(dataExt)
        , m_min(minval)
        , m_max(maxval) 
    { }
    
    virtual ~Volume() { }
    
    void extent(const BBox &e) { m_ext = e; }

    virtual void generate() = 0;  

protected:
    std::string m_outfilePath;
    BBox m_ext;
    BBox m_dataExt;
    float m_min, m_max;
};

/** \brief cube shape volume of \c size dimensions, with cube data within the extent of size-diff. 
 * Outside of \c size-diff data values are 0.0f.
 */
class CuboidVolume : public Volume {
public:    
    CuboidVolume(const BBox &ext, const BBox &dataExt, float min, float max, const std::string &outfilePath)
        : Volume(ext, dataExt, min, max, outfilePath)
    { }

    virtual ~CuboidVolume() { }

    virtual void generate() override {
        size_t voxels = m_ext.num_vox();
        float range = m_max - m_min;
        float delta = range / voxels;
        Point3 dims(m_ext.dims());

        std::cout << "Creating cuboid volume with values: \n"
            "\tMax, Min (range): " << m_max << ", " << m_min << " (" << range << ")\n"
            "\tWxHxD: " << dims.x() << "x" << dims.y() << "x" << dims.z() << "\n"
            "\tNumber of values created (voxels): " << voxels << "\n"
            "\tDelta: " << delta << std::endl;

        float val = 0.0f;
        const float zero = 0.0f;
        std::ofstream f(m_outfilePath.c_str(), std::ofstream::binary);

        for (size_t z = 0; z < dims.z(); ++z)
            for (size_t y = 0; y < dims.y(); ++y)
                for (size_t x = 0; x < dims.x(); ++x)
                {
                    if (m_dataExt.contains({ x, y, z })) {
                        f.write(reinterpret_cast<char*>(&val), sizeof(float));
                        val += delta;
                    }
                    else {
                        f.write(reinterpret_cast<const char*>(&zero), sizeof(float));
                    }
                }

        f.flush();
        f.close();
    }
};

/** \brief Sphere shaped volume filling extent with dimensions of \c size-diff.
 *	
 */
class SphericalVolume : public Volume {
public:
    SphericalVolume(const BBox &ext, const BBox &dataExt, float min, float max, const std::string &outfilePath)
        : Volume(ext, dataExt, min, max, outfilePath)
    { }

    virtual ~SphericalVolume() { }

    virtual void generate() override {
        size_t voxels = m_ext.num_vox();
        float range = m_max - m_min;
        float delta = range / m_dataExt.num_vox();
        Point3 ext_dims(m_ext.dims());

        std::cout << "Creating spherical volume with values: \n"
            "\tMax, Min (range): " << m_max << ", " << m_min << " (" << range << ")\n"
            "\tWxHxD: " << ext_dims.x() << "x" << ext_dims.y() << "x" << ext_dims.z() << "\n"
            "\tNumber of values created (voxels): " << voxels << "\n"
            "\tDelta: " << delta << std::endl;

        const float zero = 0.0f;

        std::ofstream f(m_outfilePath.c_str(), std::ofstream::binary);
        Point3 dataDims(m_dataExt.dims());
        Point3 center = (m_dataExt.min() + m_dataExt.max()) / 2.0f;
        float rad2 = (dataDims.x() / 2.0f) * (dataDims.x() / 2.0f);
        
        for (size_t z = 0; z < ext_dims.z(); ++z)
        for (size_t y = 0; y < ext_dims.y(); ++y)
        for (size_t x = 0; x < ext_dims.x(); ++x)
        {
            if (m_dataExt.contains({ x, y, z })) {
        
                float dist2 = (center.x() - x)*(center.x() - x) + 
                    (center.y() - y)*(center.y() - y) + (center.z() - z)*(center.z() - z);        
                
                if (dist2 <= rad2) {
                    f.write(reinterpret_cast<char*>(&dist2), sizeof(float));
                } else {
                    f.write(reinterpret_cast<const char*>(&zero), sizeof(float));
                }

            } else {
                f.write(reinterpret_cast<const char*>(&zero), sizeof(float));
            }
        }

        f.flush();
        f.close();
        
    }

};

void writeDatFile(const std::string &datFilePath, const std::string &fname, 
    size_t resx, size_t resy, size_t resz,
    unsigned int thickX, unsigned int thickY, unsigned int thickZ,
    const std::string &format) 
{
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

    std::cout << "usage: width height depth [minval maxval] [bx by bz]" << std::endl;
}

void usage() 
{
    usage("");
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
    
    unsigned int bX = 0;
    unsigned int bY = 0;
    unsigned int bZ = 0;

    float minval = 0;
    float maxval = std::numeric_limits<float>::max();

    if (argc > 4) {
        minval = atof(argv[4]);
    }

    if (argc > 5) {
        maxval = atof(argv[5]);
    }

    if (argc > 6) {
        bX = atoi(argv[6]);
    }
    
    if (argc > 7) {
        bY = atoi(argv[7]);
    }

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
     
    
    SphericalVolume cube(vol, inner, minval, maxval, "outfile_spherical.raw");
    cube.generate();
    writeDatFile("outfile_spherical.dat", "outfile_spherical.raw", 
        width, height, depth, 1, 1, 1, "FLOAT");
    std::cout << "El fin! (supposed success!)." << std::endl;


    return 0;
}

