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
        : m_minp(o.min())
        , m_maxp(o.max())
    { }

    ~BBox() { }

public:

    void shrink(const BBox &diff) {
        m_minp += diff.m_minp;
        m_maxp -= diff.m_maxp;
    }

    const Point3<T>& dims() { return m_dims; }

    inline size_t num_vox() const { return m_numVox; }

    inline bool contains(const Point3<T> &p) { return p < m_maxp && p > m_minp; }

    inline const Point3<T>& min() const { return m_minp; }
    inline const Point3<T>& max() const { return m_maxp; }

private:
    Point3<T> m_minp, m_maxp;
    Point3<T> m_dims;
    size_t m_numVox;
};

#endif // !bbox_h__

