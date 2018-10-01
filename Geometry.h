#ifndef Geometry_h
#define Geometry_h

#include <ArduinoSTL.h>

namespace Delta
{

  enum class GEOM
  {
    // work base radiuses
    main_basis,
    tool_basis,

    // work lever distances
    lever1,
    lever2_w,
    lever2_h,

    qty
  };

  struct Geometry
  {
    std::vector<float> m_dims;

    float get(GEOM i) const;
  };

} // namespace Delta

#endif // Geometry_h
