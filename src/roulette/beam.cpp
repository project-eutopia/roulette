#include "roulette/beam.h"

namespace roulette {
  // p3 - p0 = p2 - p1
  Beam::Beam(const ThreeVector& source, const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2, int nx, int ny) :
    m_source(source),
    m_p0(p0),
    m_u1((p1 - p0)/nx),
    m_u2((p2 - p1)/ny),
    m_nx(nx),
    m_ny(ny)
  {
  }

  ThreeVector Beam::directional_unit_vector(RandomGenerator& generator, int xi, int yi) const {
    ThreeVector direction = m_p0 + (xi + generator.uniform()) * m_u1 + (yi + generator.uniform()) * m_u2 - m_source;
    return direction / direction.magnitude();
  }

  int Beam::nx() const { return m_nx; }
  int Beam::ny() const { return m_ny; };

  const ThreeVector& Beam::source() const { return m_source; }
};
