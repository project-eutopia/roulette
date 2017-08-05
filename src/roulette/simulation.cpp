#include "roulette/simulation.h"

namespace roulette {
  Simulation::Simulation() :
    m_events()
  {
  }

  const std::vector<std::shared_ptr<Event>>& Simulation::events() const { return m_events; }
  void Simulation::add_event(std::shared_ptr<Event> event) { m_events.push_back(event); }
}
