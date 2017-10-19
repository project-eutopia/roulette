#include "roulette/sources/bifocal_source.h"
#include "roulette/geometries/geometry_factory.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
    BifocalSource::BifocalSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("BifocalSource"));

      if (!data.HasMember("origin_generator")) throw std::runtime_error("BifocalSource requires \"origin_generator\"");

      // Energy spectrum
      if (!data["origin_generator"].HasMember("spectrum")) throw std::runtime_error("BifocalSource requires \"origin_generator => spectrum\"");
      m_energy_spectrum = distributions::Spectrum(data["origin_generator"]["spectrum"]);
      if (data["origin_generator"]["spectrum"].IsString()) {
        m_energy_spectrum = distributions::Spectrum(data["origin_generator"]["spectrum"].GetString());
      }
      else {
        m_energy_spectrum = distributions::Spectrum(data["origin_generator"]["spectrum"]);
      }

      // Origin geometry
      if (!data["origin_generator"].HasMember("geometry")) throw std::runtime_error("BifocalSource requires \"origin_generator => geometry\"");
      m_origin = geometries::GeometryFactory::geometry(data["origin_generator"]["geometry"]);

      if (!data.HasMember("direction_generator")) throw std::runtime_error("BifocalSource requires \"direction_generator\"");

      // Direction geometry
      if (!data["direction_generator"].HasMember("geometry")) throw std::runtime_error("BifocalSource requires \"direction_generator => geometry\"");
      m_direction = geometries::GeometryFactory::geometry(data["direction_generator"]["geometry"]);

    }

    BifocalSource::BifocalSource(const std::shared_ptr<const geometries::Geometry> origin, const std::shared_ptr<const geometries::Geometry> direction, const distributions::Spectrum& energy_spectrum)
      :
        m_origin(origin),
        m_direction(direction),
        m_energy_spectrum(energy_spectrum)
    {}

    std::shared_ptr<Particle> BifocalSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      const ThreeVector origin = m_origin->sample(generator);
      const ThreeVector direction = m_direction->sample(generator);

      const ThreeVector u = (direction - origin).direction_unit_vector();

      return std::make_shared<Photon>(
        FourMomentum(
          energy,
          energy*u(0),
          energy*u(1),
          energy*u(2)
        ),
        origin
      );
    }
  };
};
