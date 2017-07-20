#include "roulette/distributions/rejection_sampling.h"

#include <cassert>

namespace roulette {
  namespace distributions {
    template <typename D>
    RejectionSampling<D>::RejectionSampling(RealFunction pdf, D envelope_x_distribution, RealFunction envelope_y_height) :
      m_pdf(pdf),
      m_envelope_x_distribution(envelope_x_distribution),
      m_envelope_y_height(envelope_y_height)
    {
    };

    template <typename D>
    double RejectionSampling<D>::operator()(RandomGenerator& generator) {
      double x, y;

      while(true) {
        x = m_envelope_x_distribution(generator);
        y = m_envelope_y_height(x) * generator.uniform();

        // Verify that the envelop truly covers the pdf
        assert(m_envelope_y_height(x) >= m_pdf(x));

        if (y < m_pdf(x)) return x;
      };
    };
  };
};
