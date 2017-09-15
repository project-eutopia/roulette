#include "roulette/photon.h"
#include "roulette/rotation_matrix.h"
#include "roulette/distributions/compton_scattering.h"

namespace roulette {
  const distributions::Exponential Photon::exponential = distributions::Exponential(1);
  const distributions::Spherical Photon::spherical = distributions::Spherical();

  double Photon::mass() const { return Photon::MASS; }
  int Photon::charge() const { return Photon::CHARGE; }

  Electron Photon::compton_scatter(double photon_energy, double electron_energy, double photon_theta, double electron_theta, double phi) {
    RotationMatrix rotate_to_initial;
    int xsign;

    // To avoid degenerate rotations, consider rotation matrix from (1,0,0) to photon direction,
    // or (-1,0,0) to photon direction
    if (m_momentum.px() >= 0) {
      xsign = 1;
      rotate_to_initial = RotationMatrix::rotationUtoV(ThreeVector(1,0,0), this->momentum().direction_unit_vector());
    }
    else {
      xsign = -1;
      rotate_to_initial = RotationMatrix::rotationUtoV(ThreeVector(-1,0,0), this->momentum().direction_unit_vector());
    }

    double sin_p = std::sin(photon_theta);
    double cos_p = std::cos(photon_theta);
    double sin_e = std::sin(electron_theta);
    double cos_e = std::cos(electron_theta);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    m_momentum = rotate_to_initial * FourMomentum(photon_energy, xsign * cos_p * photon_energy, sin_p * cos_phi * photon_energy, sin_p * sin_phi * photon_energy);
    m_energy = photon_energy;

    double e_momentum = std::sqrt(electron_energy*electron_energy - Electron::MASS*Electron::MASS);
    return Electron(
      rotate_to_initial * FourMomentum(electron_energy, xsign * cos_e * e_momentum, sin_e * cos_phi * e_momentum, sin_e * sin_phi * e_momentum),
      ThreeVector(this->position()(0), this->position()(1), this->position()(2)),
      m_weight
    );
  }

  void Photon::deposit_energy(SourceDose& source_dose) {
    const std::shared_ptr<Event> event = this->get_event(source_dose.generator(), source_dose.phantom());
    this->event_process(
      *event,
      source_dose.generator(),
      [&](Photon& photon, Electron& electron) {
        photon.deposit_energy(source_dose);
        electron.deposit_energy(source_dose);
      },
      [&](Electron& electron) {
        electron.deposit_energy(source_dose);
      }
    );
  }

  std::shared_ptr<Event> Photon::simulate(RandomGenerator& generator, const Phantom& phantom) {
    std::shared_ptr<Event> event = this->get_event(generator, phantom);
    this->event_process(
      *event,
      generator,
      [&](Photon& photon, Electron& electron) {
        event->add_child(photon.simulate(generator, phantom));
        event->add_child(electron.simulate(generator, phantom));
      },
      [&](Electron& electron) {
        event->add_child(electron.simulate(generator, phantom));
      }
    );
    return event;
  }

  std::shared_ptr<Event> Photon::get_event(RandomGenerator& generator, const Phantom& phantom) {
    // Roulette to eliminate low energy photons (below 10 keV)
    if (this->energy() < 10000) {
      // 1 in 3 chance of skipping
      if (generator.uniform() > 2.0/3.0) {
        return std::make_shared<Event>(
          Event::EventType::TERMINATED,
          Event::ParticleType::PHOTON,
          this->momentum(),
          this->position(),
          this->position()
        );
      }
      this->weight() *= 3.0/2.0;
    }

    ThreeVector initial_position = this->position();

    // Sample unitless depth travelled
    double depth = Photon::exponential(generator);
    bool inside = phantom.transport_photon_unitless_depth(*this, depth);
    if (!inside) {
      return std::make_shared<Event>(
        Event::EventType::NONE,
        Event::ParticleType::PHOTON,
        this->momentum(),
        initial_position,
        this->position()
      );
    }

    std::tuple<int,int,int> xyz = phantom.index_at(this->position());
    const Compound& compound = phantom.compound(std::get<0>(xyz), std::get<1>(xyz), std::get<2>(xyz));

    // Pick interaction type
    double compton = compound.photon_scattering_cross_section(this->energy());
    double photoelectric = compound.photon_absorption_cross_section(this->energy());

    if ((compton + photoelectric) * generator.uniform() < compton) {
      return std::make_shared<Event>(
        Event::EventType::PHOTON_SCATTER,
        Event::ParticleType::PHOTON,
        this->momentum(),
        initial_position,
        this->position()
      );
    }
    else {
      return std::make_shared<Event>(
        Event::EventType::PHOTON_PHOTOELECTRIC,
        Event::ParticleType::PHOTON,
        this->momentum(),
        initial_position,
        this->position()
      );
    }
  }

  void Photon::event_process(const Event& event, RandomGenerator& generator, std::function<void(Photon&, Electron&)> compton_handler, std::function<void(Electron&)> photoelectric_handler) {
    switch(event.event_type()) {
      case Event::EventType::PHOTON_SCATTER:
        {
          distributions::ComptonScattering compton_scattering;

          // Compton scatter
          compton_scattering.set_initial_photon(*this);
          compton_scattering(generator);
          Electron electron = this->compton_scatter(compton_scattering.final_photon_energy(), compton_scattering.final_electron_energy(), compton_scattering.final_photon_theta(), compton_scattering.final_electron_theta(), compton_scattering.final_phi());

          compton_handler(*this, electron);
        }
        break;
      case Event::EventType::PHOTON_PHOTOELECTRIC:
        {
          // Photoelectric effect
          Electron electron(this->energy() + Electron::MASS, Electron::MASS, Photon::spherical(generator), this->position(), this->weight());
          photoelectric_handler(electron);
        }
        break;

      default:
        break;
    }
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p) {
  os << "Photon(" << p.momentum() << ", " << p.position() << ", weight=" << p.weight() << ")";
  return os;
}
