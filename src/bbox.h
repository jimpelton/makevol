#ifndef bbox_h__
#define bbox_h__

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
        return Point3(m_maxp - m_minp);
    }


    inline size_t num_vox() const {
        Point3 diff(m_maxp - m_minp);
        return diff.x() * diff.y() * diff.z();
    }

    inline bool contains(const Point3 &p) { return p < m_maxp && p > m_minp; }

    inline const Point3& min() const { return m_minp; }
    inline const Point3& max() const { return m_maxp; }

private:
    Point3 m_minp, m_maxp;

};

#endif // !bbox_h__

