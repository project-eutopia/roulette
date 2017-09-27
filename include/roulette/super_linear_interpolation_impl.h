#include <vector>

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
    int n = N-1;
    std::vector<double> theta(n+2);
    std::vector<double> phi(n+2);

    theta[0] = 1;
    theta[1] = M(0,0);
    for (int i = 2; i <= n; ++i) {
      theta[i] = M(i-1,i-1) * theta[i-1] - M(i-2,i-1) * M(i-1,i-2) * theta[i-2];
    }

    phi[n+1] = 1;
    phi[n] = M(N-2,N-2);
    for (int i = n-1; i >= 1; --i) {
      phi[i] = M(i-1,i-1) * phi[i+1] - M(i-1,i) * M(i,i-1) * phi[i+2];
    }

    TwoTensor Minv(N-1,N-1);

    double value;
    for (int i = 1; i <= n; ++i) {
      // i > j
      for (int j = 1; j < i; ++j) {
        value = 1 - 2*((i+j) % 2);
        value *= theta[j-1] * phi[i+1] / theta[n];
        for (int k = j; k <= i-1; ++k) value *= M(k,k-1);

        Minv(i-1,j-1) = value;
      }

      // i == j
      Minv(i-1,i-1) = theta[i-1] * phi[i+1] / theta[n];

      // i < j
      for (int j = i+1; j <= n; ++j) {
        value = 1 - 2*((i+j) % 2);
        value *= theta[i-1] * phi[j+1] / theta[n];
        for (int k = i; k <= j-1; ++k) value *= M(k-1,k);

        Minv(i-1,j-1) = value;
      }
    }

    // y = Minv * C
    std::vector<double> y(N-1);
    for (int i = 0; i < N-1; ++i) {
      y[i] = 0;

      for (int j = 0; j < N-1; ++j) {
        y[i] += Minv(i,j) * C[j];
      }
    }

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
