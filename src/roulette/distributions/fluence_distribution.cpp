#include "roulette/distributions/fluence_distribution.h"

namespace roulette {
  namespace distributions {
    FluenceDistribution::FluenceDistribution() {}

    FluenceDistribution::FluenceDistribution(const TwoTensor& fluence) :
      m_nx(fluence.nx()),
      m_ny(fluence.ny())
    {
      double total_fluence = 0;
      int non_zero = 0;
      for (int i = 0; i < fluence.nx(); ++i) {
        for (int j = 0; j < fluence.ny(); ++j) {
          total_fluence += fluence(i, j);
          ++non_zero;
        }
      }

      std::vector<double> probabilities;
      probabilities.reserve(non_zero);
      m_indexes.reserve(non_zero);

      for (int i = 0; i < fluence.nx(); ++i) {
        for (int j = 0; j < fluence.ny(); ++j) {
          if (fluence(i,j) > 0) {
            probabilities.push_back(fluence(i,j) / total_fluence);
            m_indexes.push_back(std::make_pair(i, j));
          }
        }
      }

      m_inverse_cdf_to_of_indexes.add_point(0, 0);

      for (int i = 0; i < probabilities.size(); ++i) {
        m_inverse_cdf_to_of_indexes.add_point(
          probabilities[i] + m_inverse_cdf_to_of_indexes.xs().back(),
          i+1
        );
      }
    }

    int FluenceDistribution::nx() const { return m_nx; }
    int FluenceDistribution::ny() const { return m_ny; }

    std::pair<int,int> FluenceDistribution::index(RandomGenerator& generator) const {
      return m_indexes[(int)m_inverse_cdf_to_of_indexes(generator.uniform())];
    }
  };
};
