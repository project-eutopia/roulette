#pragma once

#include <stdexcept>
#include <deque>
#include <functional>
#include <mutex>

namespace roulette {
  class JobQueue {
    private:
      std::deque<std::function<void()>> m_jobs;
      std::mutex m_mutex;

    public:
      class NoJobsLeftException : public std::runtime_error {
        public:
          NoJobsLeftException();
      };

      JobQueue();

      void add_job(std::function<void()> job);
      std::function<void()> get_job();
      void run();
  };
}
