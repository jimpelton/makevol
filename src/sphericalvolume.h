#ifndef sphericalvolume_h__
#define sphericalvolume_h__

#include "point3.h"
#include "Volume.h"

#include <iostream>
#include <fstream>

/// \brief Sphere shaped volume filling extent with dimensions of \c size-diff.
template<typename T>
class SphericalVolume : public Volume<T>
{
public:
    using vol_type = Volume<T>;

    SphericalVolume(const BBox<size_t> &ext, const BBox<size_t> &dataExt, float min, float max)
        : Volume<T>(ext, dataExt)
        , m_min(min)
        , m_max(max)
        , m_vox(0)
    {
        auto extMin = ext.min();
        auto extMax = ext.max();
        m_center = (extMax + extMin) / 2;
        auto x = dataExt.dims().x();
        m_rad2 = (x * 0.5f) * (x * 0.5f);
    }

    virtual ~SphericalVolume() { }


    ////////////////////////////////////////////////////////////////////////////
    /// \brief Fill buf with distances
    ////////////////////////////////////////////////////////////////////////////
    virtual size_t next(T *buf, size_t bufsize) override
    {
        T const * const start = buf;
        T const * const end = buf + bufsize;
        while(m_vox < vol_type::numVox()) {
            if (buf>=end) {
                m_vox += buf - start;
                return buf-start;
            }

            size_t z =  m_vox %  vol_type::extZ();
            size_t y = (m_vox /  vol_type::extZ()) % vol_type::extY();
            size_t x =  m_vox / (vol_type::extY()  * vol_type::extZ());


            if (vol_type::dataExtent().contains({x,y,z})) {
                *buf = computeDistSquared(T(x), T(y), T(z));
            } else {
                *buf = 0;
            }

            ++buf;
        }

        return buf-start;
    }

private:

    /// \brief Compute distance to center.
    T computeDistSquared(T x, T y, T z)
    {
        T xdiff { m_center.x() - x };
        T ydiff { m_center.y() - y };
        T zdiff { m_center.z() - z };
        T dist2 { xdiff*xdiff + ydiff*ydiff + zdiff*zdiff };
        if (dist2 <= m_rad2)   // normalize
            return m_max * (dist2 / m_rad2) + m_min;

        return 0;
    }


    T m_min;                 ///< Minimum voxel value.
    T m_max;                 ///< Maximum voxel value.
    Point3<T> m_center;      ///< Sphere center.
    T m_rad2;                ///< Radius^2.
    size_t m_vox;            ///< Number of voxels processed so far.

}; // SphericalVolume

#endif
