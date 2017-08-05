#pragma once

#include "roulette/event.h"

namespace roulette {
  class Simulation {
    private:
      std::vector<std::shared_ptr<Event>> m_events;

    public:
      Simulation();

      const std::vector<std::shared_ptr<Event>>& events() const;
      void add_event(std::shared_ptr<Event> event);
  };
}
