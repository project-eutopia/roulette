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

  std::ofstream& Event::write(std::ofstream& ofs) const {
    ofs << "Event(" << m_event_type << ", " << m_particle_type << ", " << m_initial_momentum << ", " << m_initial_position << ", " << m_final_position;
    ofs << ", children = [";
    for (const auto child : m_children) {
      child->write(ofs);
      ofs << ", ";
    }
    ofs << "])";
    return ofs;
  }

  rapidjson::Value Event::to_json(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v;
    v.SetObject();
    v.AddMember("event_type", rapidjson::Value().SetInt(m_event_type), allocator);
    v.AddMember("particle_type", rapidjson::Value().SetInt(m_particle_type), allocator);
    v.AddMember("initial_momentum", m_initial_momentum.to_json(allocator), allocator);
    v.AddMember("initial_position", m_initial_position.to_json(allocator), allocator);
    v.AddMember("final_position", m_final_position.to_json(allocator), allocator);

    rapidjson::Value children;
    children.SetArray();

    for (auto child : m_children) {
      children.PushBack(child->to_json(allocator), allocator);
    }

    v.AddMember("children", children, allocator);

    return v;
  }
}
