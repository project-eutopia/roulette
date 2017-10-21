#include <vector>
#include <cmath>

#include "roulette/two_tensor.h"

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

    TwoTensor M(N-1, N-1);
    std::vector<double> C(N-1);

    for (int i = 0; i < N-1; ++i) {
      C[i] = -F1[i] + m_xs[i]*F0[i] + F1[i+1] - m_xs[i+2]*F0[i+1];
      M(i,i)   = -2*m_delta_x*m_delta_x/3;

      if (i == 0) {
        C[i] += y0 * m_delta_x*m_delta_x/6;
        M(i,i+1) = -m_delta_x*m_delta_x/6;
      }
      else if (i == N-2) {
        C[i] += yN * m_delta_x*m_delta_x/6;
        M(i,i-1) = -m_delta_x*m_delta_x/6;
      }
      else {
        M(i,i-1) = -m_delta_x*m_delta_x/6;
        M(i,i+1) = -m_delta_x*m_delta_x/6;
      }
    }

    // Invert tridiagonal matrix M
    // See: https://en.wikipedia.org/wiki/Tridiagonal_matrix#Inversion
    //
    // Since our matrix has the same element on the diagonal, and the same
    // element on the off-diagonal, it is possible to find an analytical
    // form for the inverse matrix by solving a second-order recursive relation
    int n = N-1;
    double r1 = 2 + std::sqrt(3);
    double r2 = 2 - std::sqrt(3);
    double d = -m_delta_x*m_delta_x/6;
    std::vector<double> a(n+2);

    a[0] = 1;
    for (int i = 1; i < n+2; ++i) {
      a[i] = (std::pow(r1, i+1) - std::pow(r2, i+1)) / 2 / std::sqrt(3);
    }

    TwoTensor Minv(n,n);

    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j <= n; ++j) {
        Minv(i-1,j-1) = (((i+j) & 1) ? -1 : 1) / d * a[std::min(i,j)-1] * a[n-std::max(i,j)] / a[n];
      }
    }

    // y = Minv * C
    m_ys[0] = y0;
    for (int i = 0; i < N-1; ++i) {
      m_ys[i+1] = 0;

      for (int j = 0; j < N-1; ++j) {
        m_ys[i+1] += Minv(i,j) * C[j];
      }
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
