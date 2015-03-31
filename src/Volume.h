#ifndef volume_h__
#define volume_h__

#include "bbox.h"

#include <string>

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

#endif
