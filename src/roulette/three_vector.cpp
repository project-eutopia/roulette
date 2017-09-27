#include "roulette/three_vector.h"

#include <stdexcept>
#include <cmath>

namespace roulette {
  ThreeVector::ThreeVector(const rapidjson::Value& data)
  {
    if (!data.IsArray() || data.Size() != 3) throw std::runtime_error("ThreeVector must be length 3 array");
    for (int i = 0; i < 3; ++i) {
      if (!data[i].IsNumber()) throw std::runtime_error("ThreeVector must be array of numbers");
      m_a[i] = data[i].GetDouble();
    }
  }

  ThreeVector::ThreeVector(double x, double y, double z)
  {
    m_a[0] = x;
    m_a[1] = y;
    m_a[2] = z;
  }

  double ThreeVector::operator()(int i) const {
    assert(i >= 0 && i < 3);
    return m_a[i];
  }

  double& ThreeVector::operator()(int i) {
    assert(i >= 0 && i < 3);
    return m_a[i];
  }

  double ThreeVector::x() const { return m_a[0]; }
  double ThreeVector::y() const { return m_a[1]; }
  double ThreeVector::z() const { return m_a[2]; }

  double ThreeVector::magnitude() const {
    return std::sqrt(this->magnitude2());
  }
  double ThreeVector::magnitude2() const {
    return m_a[0]*m_a[0] + m_a[1]*m_a[1] + m_a[2]*m_a[2];
  }

  ThreeVector ThreeVector::direction_unit_vector() const {
    return (*this) / this->magnitude();
  }

  double ThreeVector::dot(const ThreeVector& other) const {
    return (
      m_a[0]*other(0) +
      m_a[1]*other(1) +
      m_a[2]*other(2)
    );
  }

  ThreeVector ThreeVector::cross(const ThreeVector& other) const {
    return {
      m_a[1]*other(2) - m_a[2]*other(1),
      m_a[2]*other(0) - m_a[0]*other(2),
      m_a[0]*other(1) - m_a[1]*other(0)
    };
  }

  bool ThreeVector::operator==(const ThreeVector& other) const {
    return (m_a[0] == other(0)) && (m_a[1] == other(1)) && (m_a[2] == other(2));
  }

  ThreeVector ThreeVector::operator-() const {
    return -1*(*this);
  }

  ThreeVector& ThreeVector::operator+=(const ThreeVector& rhs) {
    for (int i = 0; i < 3; ++i) m_a[i] += rhs(i);
    return (*this);
  }

  ThreeVector operator+(const ThreeVector& lhs, const ThreeVector& rhs) {
    return ThreeVector(lhs(0) + rhs(0), lhs(1) + rhs(1), lhs(2) + rhs(2));
  }

  ThreeVector operator-(const ThreeVector& lhs, const ThreeVector& rhs) {
    return ThreeVector(lhs(0) - rhs(0), lhs(1) - rhs(1), lhs(2) - rhs(2));
  }

  ThreeVector operator*(const ThreeVector& lhs, double rhs) {
    return ThreeVector(lhs(0) * rhs, lhs(1) * rhs, lhs(2) * rhs);
  }

  ThreeVector operator*(double lhs, const ThreeVector& rhs) {
    return ThreeVector(lhs * rhs(0), lhs * rhs(1), lhs * rhs(2));
  }

  ThreeVector operator/(const ThreeVector& lhs, double rhs) {
    return ThreeVector(lhs(0) / rhs, lhs(1) / rhs, lhs(2) / rhs);
  }

  std::ofstream& ThreeVector::write(std::ofstream& os) const {
    float val;
    for (int i = 0; i < 3; ++i) {
      val = m_a[i];
      os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    return os;
  }

  std::ifstream& ThreeVector::read(std::ifstream& is) {
    float val;
    for (int i = 0; i < 3; ++i) {
      is.read(reinterpret_cast<char*>(&val), sizeof(val));
      m_a[i] = val;
    }
    return is;
  }

  rapidjson::Value ThreeVector::to_json(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v;
    v.SetArray();
    v.PushBack(rapidjson::Value().SetDouble(m_a[0]), allocator);
    v.PushBack(rapidjson::Value().SetDouble(m_a[1]), allocator);
    v.PushBack(rapidjson::Value().SetDouble(m_a[2]), allocator);
    return v;
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::ThreeVector& v) {
  os << "ThreeVector(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
  return os;
}

