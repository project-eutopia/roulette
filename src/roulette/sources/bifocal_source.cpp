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

      if (data.HasMember("aperture_generator")) {
        if (!data["aperture_generator"].HasMember("geometry")) throw std::runtime_error("BifocalSource[\"aperture_generator\"] requires \"geometry\"");
        m_secondary = geometries::GeometryFactory::geometry(data["aperture_generator"]["geometry"]);
        m_secondary_is_aperture_generator = true;
      }
      else if (data.HasMember("direction_generator")) {
        if (!data["direction_generator"].HasMember("geometry")) throw std::runtime_error("BifocalSource[\"direction_generator\"] requires \"geometry\"");
        m_secondary = geometries::GeometryFactory::geometry(data["direction_generator"]["geometry"]);
        m_secondary_is_aperture_generator = false;
      }
      else {
        throw std::runtime_error("BifocalSource requires either \"aperture_generator\" or \"direction_generator\"");
      }
    }

    std::shared_ptr<Particle> BifocalSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      const ThreeVector origin = m_origin->sample(generator);
      const ThreeVector secondary = m_secondary->sample(generator);

      const ThreeVector u = m_secondary_is_aperture_generator ? (secondary - origin).direction_unit_vector() : secondary.direction_unit_vector();

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
