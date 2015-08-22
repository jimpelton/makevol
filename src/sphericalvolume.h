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

    SphericalVolume(const BBox<T> &ext, const BBox<T> &dataExt, float min, float max)
        : Volume<T>(ext, dataExt)
        , m_min(min)
        , m_max(max)
    {
        auto extMin = vol_type::dataExtent().min();
        auto extMax = vol_type::dataExtent().max();
        m_center = (extMax + extMin) / 2.0f;
        auto x = vol_type::dataExtent().dims().x();
        m_rad2 = (x * 0.5f) * (x * 0.5f);
    }

    virtual ~SphericalVolume() { }


    ////////////////////////////////////////////////////////////////////////////
    /// \brief Fill buf with distances
    ////////////////////////////////////////////////////////////////////////////
    virtual size_t next(T *buf, size_t bufsize) override
    {
        T *start = buf;
        T *end = buf + bufsize;
        while(m_vox < vol_type::numVox()) {
            if (buf>=end) {
                m_vox += buf - start;
                return buf-start;
            }

            size_t z =  m_vox %  vol_type::extZ();
            size_t y = (m_vox /  vol_type::extZ()) % vol_type::extY();
            size_t x =  m_vox / (vol_type::extY()  * vol_type::extZ());


            if (vol_type::dataExtent().contains({x,y,z})) {
                *buf = computeDistSquared(x, y, z);
            } else {
                *buf = 0;
            }

            ++buf;
        }

        return buf-start;
    }

private:

    /// \brief Compute distance to center.
    T computeDistSquared(size_t x, size_t y, size_t z)
    {
        T xdiff { m_center.x() - x };
        T ydiff { m_center.y() - y };
        T zdiff { m_center.z() - z };
        T dist2 { xdiff*xdiff + ydiff*ydiff + zdiff*zdiff };
        if (dist2 <= m_rad2)   // normalize
            return m_max * (dist2 / m_rad2) + m_min;

        return 0;
    }


    T m_min;
    T m_max;
    Point3<float> m_center;  ///< Sphere center
    float m_rad2;            ///< Radius^2

    size_t m_vox;

}; // SphericalVolume

#endif
