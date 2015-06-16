#ifndef volume_h__
#define volume_h__

#include "bbox.h"

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
class Volume {
public:

    Volume(const BBox<T> &ext, const BBox<T> &dataExt, float minval, float maxval,
       const std::string &outfilePath)
       : m_outfilePath(outfilePath),
         m_voxExtent(ext),
         m_dataExt(dataExt),
         m_min(minval),
         m_max(maxval)
    { }

    virtual
    ~Volume()
    { }


    //////////////////////////////////////////////////////////////////////////
    /// \brief Fill buf with the next block of data.
    //////////////////////////////////////////////////////////////////////////
    virtual size_t
    next(T *buf, size_t bufsize) = 0;


    //////////////////////////////////////////////////////////////////////////
    /// \brief Start the volume creation.
    /// \param bufsize The amount of buffer to use when generating data.
    /// \return Total voxels written to disk.
    //////////////////////////////////////////////////////////////////////////
    size_t
    generate(size_t bufsize)
    {
        size_t voxels{ m_voxExtent.num_vox() };
        std::ofstream f{ m_outfilePath.c_str(), std::ofstream::binary };
        if (! f.is_open()){
            return 0;
        }

        print();
        size_t total{ 0 };
        T *buf { new T[bufsize] };

        while(total <= voxels) {
            size_t n{ next(buf, bufsize) };
            f.write(reinterpret_cast<char*>(buf), n * sizeof(T));
            total += n;
        }

        f.flush();
        f.close();
        delete [] buf;

        return total;
    }


    void
    print()
    {
        size_t voxels{ m_voxExtent.num_vox() };
        float range{ m_max - m_min };
        auto ext_dims = m_voxExtent.dims();

        std::cout << "Creating volume with values: \n"
                "\tMax, Min (range): " << m_max << ", " << m_min << " (" << range << ")\n"
                "\tWxHxD: " << ext_dims.x() << "x" << ext_dims.y() << "x" << ext_dims.z() << "\n"
                "\tNumber of voxels: " << voxels << "\n";
    }

    void
    extent(const BBox<T> &e)
    {
        m_voxExtent = e;
    }

protected:

    std::string m_outfilePath;
    BBox<size_t> m_voxExtent;  ///< Voxels in volume (including border)
    BBox<size_t> m_dataExt;    ///< Voxels, not including border
    T m_min, m_max;

};

#endif
