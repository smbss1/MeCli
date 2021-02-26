#ifndef MECLI_SCHEDULER_HPP_
#define MECLI_SCHEDULER_HPP_

#include <functional>

namespace mecli
{

    /**
     * A `IScheduler` represents an engine capable of running a task.
     * Its method `Post` can be safely called from any thread to submit the task
     * that will execute in an unspecified thread of execution as soon as possible
     * (but in any case after the call to `Post` is terminated).
     */
    class IScheduler
    {
    public:
        virtual ~IScheduler() = default;

        /// Submits a completion token or function object for execution.
        virtual void Post(const std::function<void()>& f) = 0;
    };

} // namespace mecli

#endif // CLI_SCHEDULER_H_