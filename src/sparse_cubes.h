

#ifndef sparse_cubes_h__
#define sparse_cubes_h__

#include "Volume.h"

#include <vector>

template<typename T>
class SparseCubes : public Volume<T>
{

public:
    using vol_type = Volume<T>;

    SparseCubes(BBox<size_t> const & ext, BBox<size_t> const &dataExt) 
        : Volume<T>(ext, dataExt) 
        , m_vox{ 0 } 
        , m_voxelGroupSizeMin{ 0 }
        , m_voxelGroupSizeMax{ 0 }
        , m_group{ 0 }
    { }
    

    virtual ~SparseCubes() { }


    virtual size_t next(T *buf, size_t bufsize) override 
    { 
        T const * const start = buf;
        T const * const end = buf + bufsize;
        while (buf < end && m_vox < vol_type::numVox()) {
            
        }
        
    }

    
    /////////////////////////////////////////////////////////////////////////// 
    void voxelGroupSizeMin(size_t sz) 
    {
        m_voxelGroupSizeMin = sz;
    }

    
    /////////////////////////////////////////////////////////////////////////// 
    void voxelGroupSizeMax(size_t sz) 
    {
        m_voxelGroupSizeMax = sz;
    }


    /////////////////////////////////////////////////////////////////////////// 
    /// \brief Clear the groups and reserve space for \c n groups.
    /////////////////////////////////////////////////////////////////////////// 
    void resetGroups(size_t n) {
        m_groups.clear();
        m_groups.reserve(n);
    }
    

private:
    size_t m_vox;
    size_t m_voxelGroupSizeMin;
    size_t m_voxelGroupSizeMax;

    size_t m_group;  ///< Current group index;

    std::vector<BBox<size_t>> m_groups;  ///< Location of voxel groups.

}; 

#endif // !sparse_cubes_h__

