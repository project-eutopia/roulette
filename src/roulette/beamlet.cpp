#include "roulette/beamlet.h"

#include <cassert>

namespace roulette {
  // p3 - p0 = p2 - p1
  Beamlet::Beamlet(const ThreeVector& source, const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2) :
    m_source(source),
    m_p0(p0),
    m_u1(p1 - p0),
    m_u2(p2 - p1)
  {
    // Ensure non-degenerate
    assert(m_u1.cross(m_u2).magnitude2() != 0);
  }

  ThreeVector Beamlet::directional_unit_vector(RandomGenerator& generator) const {
    ThreeVector direction = m_p0 + generator.uniform() * m_u1 + generator.uniform() * m_u2 - m_source;
    return direction / direction.magnitude();
  }

  const ThreeVector& Beamlet::source() const { return m_source; }
};
