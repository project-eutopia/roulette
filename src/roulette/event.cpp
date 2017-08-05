#include "roulette/event.h"

namespace roulette {
  Event::Event(
    EventType event_type,
    ParticleType particle_type,
    const FourMomentum& initial_momentum,
    const ThreeVector& initial_position,
    const ThreeVector& final_position
  ) :
    m_event_type(event_type),
    m_particle_type(particle_type),
    m_initial_momentum(initial_momentum),
    m_initial_position(initial_position),
    m_final_position(final_position)
  {
  }

  Event::EventType Event::event_type() const { return m_event_type; }
  Event::ParticleType Event::particle_type() const { return m_particle_type; }
  const FourMomentum& Event::initial_momentum() const { return m_initial_momentum; }
  const ThreeVector& Event::initial_position() const { return m_initial_position; }
  const ThreeVector& Event::final_position() const { return m_final_position; }

  const std::vector<std::shared_ptr<Event>>& Event::children() const { return m_children; }
  void Event::add_child(std::shared_ptr<Event> child) {
    m_children.push_back(child);
  }
}
