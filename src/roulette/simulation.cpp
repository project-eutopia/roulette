#include "roulette/simulation.h"

namespace roulette {
  Simulation::Simulation() :
    m_events()
  {
  }

  const std::vector<std::shared_ptr<Event>>& Simulation::events() const { return m_events; }
  void Simulation::add_event(std::shared_ptr<Event> event) { m_events.push_back(event); }

  std::ofstream& Simulation::write(std::ofstream& ofs) const {
    ofs << "Simulation[" << std::endl;
    for (auto event : m_events) {
      ofs << "  ";
      event->write(ofs);
      ofs << std::endl;
    }
    ofs << "]";
    return ofs;
  }
}
