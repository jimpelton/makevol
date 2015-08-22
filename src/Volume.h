#ifndef volume_h__
#define volume_h__

#include "bbox.h"

#include <iostream>
#include <fstream>
#include <string>

template<typename T>
class Volume {
public:

    Volume(const BBox<size_t> &ext, const BBox<size_t> &dataExt)
       : m_outfilePath(""),
         m_voxExtent(ext),
         m_dataExt(dataExt)
    { }

    virtual ~Volume()
    { }


    //////////////////////////////////////////////////////////////////////////
    /// \brief Fill buf with the next block of data.
    //////////////////////////////////////////////////////////////////////////
    virtual size_t next(T *buf, size_t bufsize) = 0;


    //////////////////////////////////////////////////////////////////////////
    /// \brief Start the volume creation.
    /// \param bufsize The amount of buffer to use when generating data.
    /// \return Total voxels written to disk.
    //////////////////////////////////////////////////////////////////////////
    size_t generate(size_t bufsize)
    {
        size_t voxels{ m_voxExtent.num_vox() };
        std::ofstream f{ m_outfilePath.c_str(), std::ofstream::binary };
        if (! f.is_open()){
            return 0;
        }

        //print();
        size_t total{ 0 };
        T *buf { new T[bufsize] };

        while(total <= voxels) {
            size_t voxelsWritten{ next(buf, bufsize) };
            f.write(reinterpret_cast<char*>(buf), voxelsWritten * sizeof(T));
            total += voxelsWritten;
            std::cout << voxelsWritten << ", " << total << std::endl;
        }

        f.flush();
        f.close();
        delete [] buf;

        return total;
    }


//    virtual void toString()
//    {
//        size_t voxels{ m_voxExtent.num_vox() };
//        //float range{ m_max - m_min };
//        auto ext_dims = m_voxExtent.dims();
//
//        std::cout << "Creating volume with values: \n"
//                "\tMax, Min (range): " << m_max << ", " << m_min << " (" << range << ")\n"
//                "\tWxHxD: " << ext_dims.x() << "x" << ext_dims.y() << "x" << ext_dims.z() << "\n"
//                "\tNumber of voxels: " << voxels << "\n";
//    }


    //////////////////////////////////////////////////////////////////////////
    /// \brief Set the path to the output file.
    //////////////////////////////////////////////////////////////////////////
    void outfile(std::string const &path)
    {
        m_outfilePath = path;
    }


protected:
    size_t extZ() const { return m_voxExtent.dims().z(); }
    size_t extY() const { return m_voxExtent.dims().y(); }
    size_t extX() const { return m_voxExtent.dims().x(); }
    size_t numVox() const { return m_voxExtent.num_vox(); }

    const BBox<size_t> & dataExtent() const { return m_dataExt; }

private:
    std::string m_outfilePath;
    BBox<size_t> m_voxExtent;  ///< Voxels in volume (including border)
    BBox<size_t> m_dataExt;    ///< Voxels, not including border
};

#endif
