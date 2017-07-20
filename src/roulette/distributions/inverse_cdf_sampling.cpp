#include "roulette/distributions/inverse_cdf_sampling.h"

namespace roulette {
  namespace distributions {
    InverseCdfSampling::InverseCdfSampling(RealFunction inv_cdf) :
      m_inv_cdf(inv_cdf)
    {
    };

    double InverseCdfSampling::operator()(RandomGenerator& generator) {
      return m_inv_cdf(generator.uniform());
    };
  };
};
