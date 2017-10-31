namespace roulette {
  namespace distributions {

    template <typename T>
    ProbabilitySampling<T>::ProbabilitySampling() {
    }

    template <typename T>
    ProbabilitySampling<T>::ProbabilitySampling(const std::vector<double>& probabilities, const std::vector<T>& items) {
      assert(probabilities.size() == items.size());
      assert(probabilities.size() > 0);

      double total_probability = 0;
      size_t non_zero = 0;

      for (double probability : probabilities) {
        if (probability > 0) {
          total_probability += probability;
          ++non_zero;
        }
      }

      std::vector<double> non_zero_probabilities;
      non_zero_probabilities.reserve(non_zero);
      m_values.reserve(non_zero);

      for (size_t i = 0; i < probabilities.size(); ++i) {
        if (probabilities[i] > 0) {
          non_zero_probabilities.push_back(probabilities[i] / total_probability);
          m_values.push_back(items[i]);
        }
      }

      m_inv_cdf_to_indexes.add_point(0, 0);

      for (size_t i = 0; i < non_zero_probabilities.size(); ++i) {
        m_inv_cdf_to_indexes.add_point(
          non_zero_probabilities[i] + m_inv_cdf_to_indexes.xs().back(),
          i+1
        );
      }
    }

    template <typename T>
    const T& ProbabilitySampling<T>::operator()(RandomGenerator& generator) const {
      return m_values[(size_t)m_inv_cdf_to_indexes(generator.uniform())];
    }
  };
};
