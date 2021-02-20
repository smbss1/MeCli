#ifndef CLI_DETAIL_INPUTDEVICE_HPP_
#define CLI_DETAIL_INPUTDEVICE_HPP_

#include <functional>
#include <string>

#include "IScheduler.hpp"

namespace cli
{
namespace detail
{

enum class KeyType { ascii, up, down, left, right, backspace, canc, home, end, ret, eof, ignored };

class InputDevice
{
public:
    using Handler = std::function< void( std::pair<KeyType,char> ) >;

    InputDevice(IScheduler& _scheduler) : scheduler(_scheduler) {}
    virtual ~InputDevice() = default;

    template <typename H>
    void Register(H&& h) { handler = std::forward<H>(h); }

protected:

    void Notify(std::pair<KeyType,char> k)
    {
        scheduler.Post([this,k](){ if (handler) handler(k); });
    }

private:

    IScheduler& scheduler;
    Handler handler;
};

} // namespace detail
} // namespace cli

#endif // CLI_DETAIL_INPUTDEVICE_H_
