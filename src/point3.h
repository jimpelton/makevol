#ifndef point3_h__
#define point3_h__


class Point3 {
public:
    Point3() : m_x(0), m_y(0), m_z(0) { }

    Point3(size_t x, size_t y, size_t z) : m_x(x), m_y(y), m_z(z) { }

    explicit Point3(size_t *l) : m_x(l[0]), m_y(l[1]), m_z(l[2]) { }

    Point3(const Point3 &o) : m_x(o.m_x), m_y(o.m_y), m_z(o.m_z) { }

    ~Point3() { }

    inline friend bool operator<(const Point3 &lhs, const Point3 &rhs)
    {
        return lhs.m_x < rhs.m_x && lhs.m_y < rhs.m_y && lhs.m_z < rhs.m_z;
    }

    inline friend bool operator>(const Point3 &lhs, const Point3 &rhs)
    {
        return rhs < lhs;
    }

    inline friend bool operator<=(const Point3 &lhs, const Point3 &rhs)
    {
        return !(lhs > rhs);
    }

    inline friend bool operator>=(const Point3 &lhs, const Point3 &rhs)
    {
        return !(lhs < rhs);
    }

    Point3& operator-=(const Point3 &rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;

        return *this;
    }

    const Point3 operator-(const Point3 &v) const
    {
        Point3 rval(*this);
        rval -= v;

        return rval;
    }

    Point3& operator+=(const Point3 &rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;

        return *this;
    }

    Point3 operator+(const Point3 &v) const
    {
        Point3 rval(*this);
        rval += v;

        return rval;
    }

    Point3& operator/=(const Point3 &rhs)
    {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
        m_z /= rhs.m_z;

        return *this;
    }

    Point3 operator/(const Point3 &v) const
    {
        Point3 rval(*this);
        rval /= v;

        return rval;
    }

    Point3 operator/(float s) const
    {
        Point3 rval(*this);
        rval /= Point3(s, s, s);

        return rval;
    }

    size_t x() const
    {
        return m_x;
    }

    size_t y() const
    {
        return m_y;
    }

    size_t z() const
    {
        return m_z;
    }

private:
    size_t m_x, m_y, m_z;
};


#endif
