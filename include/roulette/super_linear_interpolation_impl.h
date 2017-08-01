#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace roulette {
  template <typename F>
  SuperLinearInterpolation<F>::SuperLinearInterpolation(F f, std::function<double(double)> integral_f, std::function<double(double)> integral_xf, double x0, double xN, int N) :
    m_f(f),
    m_xs(N+1),
    m_ys(N+1),
    m_delta_x((xN - x0) / N)
  {
    double y0 = f(x0);
    double yN = f(xN);

    for (int i = 0; i <= N; ++i) {
      m_xs[i] = x0 + i * m_delta_x;
    }

    std::vector<double> delta2_x(N);
    std::vector<double> delta3_x(N);
    std::vector<double> a1(N);
    std::vector<double> b1(N);
    std::vector<double> a2(N);
    std::vector<double> b2(N);
    std::vector<double> F1(N);
    std::vector<double> F2(N);

    for (int i = 0; i < N; ++i) {
      delta2_x[i] = m_xs[i+1]*m_xs[i+1]/2.0 - m_xs[i]*m_xs[i]/2.0;
      delta3_x[i] = m_xs[i+1]*m_xs[i+1]*m_xs[i+1]/3.0 - m_xs[i]*m_xs[i]*m_xs[i]/3.0;

      a1[i] = delta2_x[i]/m_delta_x - m_xs[i];
      b1[i] = m_xs[i]+m_delta_x - delta2_x[i]/m_delta_x;
      a2[i] = delta3_x[i]/m_delta_x - delta2_x[i]*m_xs[i]/m_delta_x;
      b2[i] = delta2_x[i]*m_xs[i]/m_delta_x + delta2_x[i] - delta3_x[i]/m_delta_x;

      F1[i] = integral_f(m_xs[i+1]) - integral_f(m_xs[i]);
      F2[i] = integral_xf(m_xs[i+1]) - integral_xf(m_xs[i]);
    }

    std::vector<double> Cprime(N+1);

    for (int i = 1; i < N; ++i) {
      Cprime[i] = -F2[i-1] + m_xs[i-1]*F1[i-1] + F2[i] - m_xs[i+1]*F1[i];
    }

    boost::numeric::ublas::matrix<double> Mprime = boost::numeric::ublas::zero_matrix<double>(N+1, N+1);

    for (int i = 1; i < N; ++i) {
      Mprime(i, i-1) = b1[i-1]*m_xs[i-1] - b2[i-1];
      Mprime(i, i) = a1[i-1]*m_xs[i-1] - a2[i-1] - b1[i]*m_xs[i+1] + b2[i];
      Mprime(i, i+1) = -a1[i]*m_xs[i+1] + a2[i];
    }

    boost::numeric::ublas::matrix<double> M = boost::numeric::ublas::zero_matrix<double>(N-1, N-1);
    boost::numeric::ublas::vector<double> C = boost::numeric::ublas::zero_vector<double>(N-1);

    for (int i = 0; i < N-1; ++i) {
      if (i == 0) {
        C(i) = Cprime[i+1] - Mprime(1,0)*y0;
      }
      else if (i == N-2) {
        C(i) = Cprime[i+1] - Mprime(N-1,N)*yN;
      }
      else {
        C(i) = Cprime[i+1];
      }

      for (int j = 0; j < N-1; ++j) {
        M(i,j) = Mprime(i+1, j+1);
      }
    }

	// create a working copy of the input
    boost::numeric::ublas::matrix<double> Mcopy(M);

	// create a permutation matrix for the LU-factorization
    boost::numeric::ublas::permutation_matrix<std::size_t> pm(M.size1());

	// perform LU-factorization
	int res = boost::numeric::ublas::lu_factorize(Mcopy, pm);
    if (res != 0) throw std::runtime_error("Couldn't LU factorize matrix in SuperLinearInterpolation");

	// create identity matrix of "inverse"
    boost::numeric::ublas::matrix<double> inverse = boost::numeric::ublas::identity_matrix<double>(Mcopy.size1());

	// backsubstitute to get the inverse
    boost::numeric::ublas::lu_substitute(Mcopy, pm, inverse);

    boost::numeric::ublas::vector<double> y = boost::numeric::ublas::prod(inverse, C);

    m_ys[0] = y0;
    for (int i = 0; i < N-1; ++i) {
      m_ys[i+1] = y[i];
    }
    m_ys[N] = yN;
  }

  template <typename F>
  const std::vector<double>& SuperLinearInterpolation<F>::xs() const {
    return m_xs;
  }

  template <typename F>
  const std::vector<double>& SuperLinearInterpolation<F>::ys() const {
    return m_ys;
  }

  template <typename F>
  double SuperLinearInterpolation<F>::operator()(double x) const {
    if (x < m_xs.front() || x >= m_xs.back()) return m_f(x);
    int i = (int)((x - m_xs.front()) / m_delta_x);

    return m_ys[i] + (x - m_xs[i]) * (m_ys[i+1] - m_ys[i]) / m_delta_x;
  }
};
