#ifndef cuboidvolume_h__
#define cuboidvolume_h__

#include "Volume.h"
#include "point3.h"

#include <iostream>
#include <fstream>

/** \brief cube shape volume of \c size dimensions, with cube data within the extent of size-diff.
* Outside of \c size-diff data values are 0.0f.
*/
class CuboidVolume : public Volume {
public:
    CuboidVolume(const BBox &ext, const BBox &dataExt, 
        float min, float max, 
        const std::string &outfilePath)
        : Volume(ext, dataExt, min, max, outfilePath)
    { }

    virtual ~CuboidVolume() { }

    virtual void generate() override 
    {
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
            } else {
                f.write(reinterpret_cast<const char*>(&zero), sizeof(float));
            }
        }

        f.flush();
        f.close();
    }
};

#endif
