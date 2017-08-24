#pragma once

#include "roulette/three_tensor.h"
#include "roulette/phantom.h"
#include "rapidjson/document.h"

#include <map>
#include <vector>
#include <string>

namespace roulette {
  class PointwiseThreeTensor : public ThreeTensor {
    private:
      std::shared_ptr<const Phantom> m_phantom;
      int m_nx;
      int m_ny;
      int m_nz;

      // Used as a "dummy" variable for referencing points that aren't considered
      double m_dummy;

      std::vector<ThreeVector> m_points;

      std::map<int,double> m_data;

    public:
      PointwiseThreeTensor();
      PointwiseThreeTensor(std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data, double fill_value = 0);

      int nx() const;
      int ny() const;
      int nz() const;
      int size() const;

      double operator()(int xi, int yi, int zi) const;
      double& operator()(int xi, int yi, int zi);

      double value_at(const ThreeVector& position) const;

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);

      void rescale(double weight, std::shared_ptr<const ThreeTensor> densities);

    private:
      int internal_index(int xi, int yi, int zi) const;
  };
};
