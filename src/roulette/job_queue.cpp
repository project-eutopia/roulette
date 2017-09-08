#include "roulette/job_queue.h"

#include <vector>
#include <thread>

namespace roulette {
  JobQueue::NoJobsLeftException::NoJobsLeftException() : std::runtime_error("No jobs left in queue")
  {}

  JobQueue::JobQueue()
  {}

  void JobQueue::add_job(std::function<void()> job) {
    m_jobs.push_back(job);
  }

  std::function<void()> JobQueue::get_job() {
    std::function<void()> job;

    m_mutex.lock();
    if (m_jobs.empty()) {
      throw NoJobsLeftException();
    }
    job = m_jobs.front();
    m_jobs.pop_front();
    m_mutex.unlock();

    return job;
  }

  void JobQueue::run() {
    std::vector<std::thread> worker_threads;

    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
      worker_threads.emplace_back(
        [this]() {
          try {
            while (true) this->get_job()();
          }
          catch (const JobQueue::NoJobsLeftException& e) {
            // break out of loop
          }
        }
      );
    }

    for (auto& t : worker_threads) {
      t.join();
    }
  }
}
