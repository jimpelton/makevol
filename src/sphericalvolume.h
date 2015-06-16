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

    SphericalVolume( const BBox<T> &ext,
        const BBox<T> &dataExt,
        float min,
        float max,
        const std::string &outfilePath )
        : Volume<T>(ext, dataExt, min, max, outfilePath)
        , m_curZ{0}
        , m_curY{0}
        , m_curX{0}
    {
        m_center = (vol_type::m_dataExt.min() + vol_type::m_dataExt.max()) / 2.0f;
        Point3<T> dims{ vol_type::m_dataExt.dims() };
        m_rad2 = (dims.x() / 2.0f) * (dims.x() / 2.0f);
    }

    virtual
    ~SphericalVolume() { }

    /// \brief Fill buf with distances
    virtual size_t
    next(T *buf, size_t bufsize) override
    {
        T *start = buf;
        T *end = buf + bufsize;
        for ( ; m_curZ < vol_type::m_voxExtent.dims().z(); ++m_curZ)
        for ( ; m_curY < vol_type::m_voxExtent.dims().y(); ++m_curY)
        for ( ; m_curX < vol_type::m_voxExtent.dims().x(); ++m_curX) {
            if (buf>=end) {
                return buf-start;
            }

            if (vol_type::m_dataExt.contains({m_curX, m_curY, m_curZ})) {
                *buf = computeDistSquared();
            } else {
                *buf = 0;
            }

            ++buf;
        }

        return buf-start;
    }

private:

    /// \brief Compute distance to center.
    T
    computeDistSquared()
    {
        T xdiff { m_center.x() - m_curX };
        T ydiff { m_center.y() - m_curY };
        T zdiff { m_center.z() - m_curZ };
        T dist2 { xdiff*xdiff + ydiff*ydiff + zdiff*zdiff };
        if (dist2 <= m_rad2)   // normalize
            return vol_type::m_max * (dist2 / m_rad2) + vol_type::m_min;

        return 0;
    }


    size_t m_curZ;
    size_t m_curY;
    size_t m_curX;
    Point3<float> m_center;  ///< Sphere center
    float m_rad2;     ///< Radius^2

}; // SphericalVolume

#endif
