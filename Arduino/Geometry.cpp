#include "Geometry.h"

namespace Delta
{
  
  float Geometry::get(GEOM i) const
  {
    return m_dims[size_t(i)];
  }

} // namespace Delta
