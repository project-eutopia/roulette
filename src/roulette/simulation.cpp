#include "roulette/simulation.h"

#include "rapidjson/document.h"

namespace roulette {
  Simulation::Simulation() :
    m_events()
  {
  }

  const std::vector<std::shared_ptr<Event>>& Simulation::events() const { return m_events; }
  void Simulation::add_event(std::shared_ptr<Event> event) { m_events.push_back(event); }

  rapidjson::Value Simulation::to_json(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v;
    v.SetObject();

    rapidjson::Value events;
    events.SetArray();

    for (auto event : m_events) {
      events.PushBack(event->to_json(allocator), allocator);
    }

    v.AddMember("events", events, allocator);
    return v;
  }
}
