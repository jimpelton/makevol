#ifndef rubiksvolume_h__
#define rubiksvolume_h__

#include "Volume.h"

template<typename T>
class RubiksVolume : public Volume<T>
{
public:

  using vol_type = Volume<T>;


  RubiksVolume(const BBox<size_t> &ext, const BBox<size_t> &dataExt) 
    : Volume(ext, dataExt)
  {

  }


  virtual size_t 
  next(T* buf, size_t bufsize) override
  {
    T const * const start = buf;
    T const * const end = buf + bufsize;
    while (buf < end && m_vox < vol_type::numVox()) {

    }
  }

private:
  size_t m_vox; ///< Number of voxels processed so far.
};


#endif // rubiksvolume_h__
