#ifndef CLI_LOOPSCHEDULER_HPP_
#define CLI_LOOPSCHEDULER_HPP_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "IScheduler.hpp"

namespace cli
{

/**
 * @brief The LoopScheduler is a simple thread-safe scheduler
 * 
 */
class LoopScheduler : public IScheduler
{
public:
    LoopScheduler() = default;
    ~LoopScheduler()
    {
        Stop();
    }

    // non copyable
    LoopScheduler(const LoopScheduler&) = delete;
    LoopScheduler& operator=(const LoopScheduler&) = delete;

    void Stop()
    {
        std::lock_guard<std::mutex> lck (mtx);
        running = false;
        cv.notify_all();
    }
    void Run()
    {
        while( ExecOne() ) {};
    }

    void Post(const std::function<void()>& f) override
    {
        std::lock_guard<std::mutex> lck (mtx);
        tasks.push(f);
        cv.notify_all();
    }

    bool ExecOne()
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, [this](){ return !running || !tasks.empty(); });
            if (!running)
                return false;
            task = tasks.front();
            tasks.pop();
        }

        if (task)
            task();

        return true;
    }
private:
    std::queue<std::function<void()>> tasks;
    bool running{ true };
    std::mutex mtx;
    std::condition_variable cv;
};

} // namespace cli

#endif // CLI_LOOPSCHEDULER_H_