#include "roulette/distributions/fluence_distribution.h"

namespace roulette {
  namespace distributions {
    FluenceDistribution::FluenceDistribution() {}

    FluenceDistribution::FluenceDistribution(const TwoTensor& fluence) :
      m_nx(fluence.nx()),
      m_ny(fluence.ny())
    {
      std::vector<double> probabilities;
      std::vector<std::pair<int,int>> indexes;

      for (int i = 0; i < fluence.nx(); ++i) {
        for (int j = 0; j < fluence.ny(); ++j) {
          if (fluence(i,j) > 0) {
            probabilities.push_back(fluence(i,j));
            indexes.push_back(std::make_pair(i, j));
          }
        }
      }

      m_sampling = ProbabilitySampling<std::pair<int,int>>(probabilities, indexes);
    }

    int FluenceDistribution::nx() const { return m_nx; }
    int FluenceDistribution::ny() const { return m_ny; }

    const std::pair<int,int>& FluenceDistribution::index(RandomGenerator& generator) const {
      return m_sampling(generator);
    }
  };
};
