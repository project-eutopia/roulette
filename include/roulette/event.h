#pragma once

#include <fstream>

#include "rapidjson/document.h"

#include "roulette/four_momentum.h"
#include "roulette/three_vector.h"

namespace roulette {
  class Event {
    public:
      enum EventType {
        NONE = 0,
        TERMINATED = 1,
        PHOTON_SCATTER = 10,
        PHOTON_PHOTOELECTRIC = 11,
        ELECTRON_ABSORB = 20
      };

      enum ParticleType {
        PHOTON = 0,
        ELECTRON = -1
      };

    private:
      EventType m_event_type;
      ParticleType m_particle_type;

      FourMomentum m_initial_momentum;
      ThreeVector m_initial_position;
      ThreeVector m_final_position;
      std::vector<std::shared_ptr<Event>> m_children;

    public:
      Event(
        EventType event_type,
        ParticleType particle_type,
        const FourMomentum& initial_momentum,
        const ThreeVector& initial_position,
        const ThreeVector& final_position
      );

      EventType event_type() const;
      ParticleType particle_type() const;
      const FourMomentum& initial_momentum() const;
      const ThreeVector& initial_position() const;
      const ThreeVector& final_position() const;

      const std::vector<std::shared_ptr<Event>>& children() const;
      void add_child(std::shared_ptr<Event> child);
      std::ofstream& write(std::ofstream& ofs) const;
      rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const;
  };
}
