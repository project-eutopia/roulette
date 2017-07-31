namespace roulette {
  namespace distributions {

    template <typename T>
    ProbabilitySampling<T>::ProbabilitySampling() {
    }

    template <typename T>
    ProbabilitySampling<T>::ProbabilitySampling(const std::vector<double>& probabilities, const std::vector<T>& outputs) {
      assert(probabilities.size() == outputs.size());
      assert(probabilities.size() > 0);

      double total_probability = 0;
      int non_zero = 0;

      for (double probability : probabilities) {
        if (probability > 0) {
          total_probability += total_probability;
          ++non_zero;
        }
      }

      std::vector<double> non_zero_probabilities;
      non_zero_probabilities.reserve(non_zero);
      m_values.reserve(non_zero);

      for (int i = 0; i < probabilities.size(); ++i) {
        if (probabilities[i] > 0) {
          non_zero_probabilities.push_back(probabilities[i] / total_probability);
          m_values.push_back(outputs[i]);
        }
      }

      m_inv_cdf_to_indexes.add_point(0, 0);

      for (int i = 0; i < non_zero_probabilities.size(); ++i) {
        m_inv_cdf_to_indexes.add_point(
          non_zero_probabilities[i] + m_inv_cdf_to_indexes.xs().back(),
          i+1
        );
      }
    }

    template <typename T>
    const T& ProbabilitySampling<T>::operator()(RandomGenerator& generator) const {
      return m_values[(int)m_inv_cdf_to_indexes(generator.uniform())];
    }
  };
};
