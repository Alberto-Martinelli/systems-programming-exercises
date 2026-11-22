#ifndef SYSTEMS_EXERCISES_JOINING_THREAD_HPP
#define SYSTEMS_EXERCISES_JOINING_THREAD_HPP

#include <thread>
#include <utility>

/* Scope-bound joining for standard libraries without std::jthread.
 * No cancellation/stop-token support is implied. Keep referenced data alive
 * until this object is destroyed; a worker must never destroy its own owner. */
class JoiningThread {
public:
    JoiningThread() noexcept = default;
    template <class Function, class... Arguments>
    explicit JoiningThread(Function&& function, Arguments&&... arguments)
        : thread_(std::forward<Function>(function),
                  std::forward<Arguments>(arguments)...) {}
    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;
    JoiningThread(JoiningThread&&) noexcept = default;
    JoiningThread& operator=(JoiningThread&& other) noexcept {
        if (this != &other) {
            if (thread_.joinable()) thread_.join();
            thread_ = std::move(other.thread_);
        }
        return *this;
    }
    ~JoiningThread() {
        if (thread_.joinable()) thread_.join();
    }
private:
    std::thread thread_;
};

#endif
