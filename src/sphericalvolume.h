#ifndef sphericalvolume_h__
#define sphericalvolume_h__

#include "point3.h"
#include "Volume.h"

#include <iostream>
#include <fstream>

/** \brief Sphere shaped volume filling extent with dimensions of \c size-diff.
*
*/
class SphericalVolume : public Volume {
public:
    SphericalVolume(const BBox &ext, const BBox &dataExt, 
        float min, float max, 
        const std::string &outfilePath)
        : Volume(ext, dataExt, min, max, outfilePath) { }

    virtual ~SphericalVolume() { }

    virtual void generate() override 
    {
        size_t voxels = m_ext.num_vox();
        Point3 ext_dims(m_ext.dims());
        float range = m_max - m_min;
        std::cout << "Creating spherical volume with values: \n"
            "\tMax, Min (range): " << m_max << ", " << m_min << " (" << range << ")\n"
            "\tWxHxD: " << ext_dims.x() << "x" << ext_dims.y() << "x" << ext_dims.z() << "\n"
            "\tNumber of values created (voxels): " << voxels << "\n";
            //"\tDelta: " << delta << std::endl;

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
                    // normalize
                    dist2 = m_max * (dist2 / rad2) + m_min;
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

#endif
