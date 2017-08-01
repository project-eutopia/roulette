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

    std::vector<double> F0(N);
    std::vector<double> F1(N);

    for (int i = 0; i < N; ++i) {
      F0[i] = integral_f(m_xs[i+1]) - integral_f(m_xs[i]);
      F1[i] = integral_xf(m_xs[i+1]) - integral_xf(m_xs[i]);
    }

    boost::numeric::ublas::matrix<double> M = boost::numeric::ublas::zero_matrix<double>(N-1, N-1);
    boost::numeric::ublas::vector<double> C = boost::numeric::ublas::zero_vector<double>(N-1);

    for (int i = 0; i < N-1; ++i) {
      C(i) = -F1[i] + m_xs[i]*F0[i] + F1[i+1] - m_xs[i+2]*F0[i+1];
      M(i,i)   = -2*m_delta_x*m_delta_x/3;

      if (i == 0) {
        C(i) += y0 * m_delta_x*m_delta_x/6;
        M(i,i+1) = -m_delta_x*m_delta_x/6;
      }
      else if (i == N-2) {
        C(i) += yN * m_delta_x*m_delta_x/6;
        M(i,i-1) = -m_delta_x*m_delta_x/6;
      }
      else {
        M(i,i-1) = -m_delta_x*m_delta_x/6;
        M(i,i+1) = -m_delta_x*m_delta_x/6;
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
