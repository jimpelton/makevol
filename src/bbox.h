#ifndef bbox_h__
#define bbox_h__

#include "point3.h"

template<typename T>
class BBox {
public:
    BBox() { }

    BBox(const Point3<T> &min, const Point3<T> &max)
        : m_minp(min)
        , m_maxp(max)
        , m_dims(max - min)
        , m_numVox(m_dims.x() * m_dims.y() * m_dims.z())
    { }

    BBox(const BBox &o)
        : m_minp(o.m_minp)
        , m_maxp(o.m_maxp)
        , m_dims(o.m_dims)
        , m_numVox(o.m_numVox)
    { }

    template<typename U>
    BBox(const BBox<U> &o)
        : m_minp(o.min())
        , m_maxp(o.max())
        , m_dims(o.dims())
        , m_numVox(o.num_vox())
    { }

    ~BBox() { }

public:

    void shrink(const BBox &diff) {
        m_minp += diff.m_minp;
        m_maxp -= diff.m_maxp;
    }

    Point3<T> const & dims() const { return m_dims; }

    inline size_t num_vox() const { return m_numVox; }

    inline bool contains(const Point3<T> &p) const { return p < m_maxp && p > m_minp; }

    inline Point3<T> const & min() const { return m_minp; }
    inline Point3<T> const & max() const { return m_maxp; }

private:
    Point3<T> m_minp, m_maxp;
    Point3<T> m_dims;
    size_t m_numVox;
};

#endif // !bbox_h__

