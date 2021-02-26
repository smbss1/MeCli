#ifndef MECLI_DETAIL_LINUXKEYBOARD_HPP_
#define MECLI_DETAIL_LINUXKEYBOARD_HPP_

#include <thread>
#include <memory>
#include <atomic>

#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <iostream>

#include "InputDevice.hpp"
#include "style.hpp"

namespace mecli
{
    namespace detail
    {
        class LinuxKeyboard : public InputDevice
        {
        public:
            explicit LinuxKeyboard(IScheduler& _scheduler) :
                    InputDevice(_scheduler)
            {
                ToManualMode();
                servant = std::make_unique<std::thread>( [this](){ Read(); } );
                servant->detach();
            }
            
            ~LinuxKeyboard()
            {
                run = false;
                ToStandardMode();
            }

        private:

            void Read()
            {
                while ( run )
                {
                    auto k = Get();
                    Notify(k);
                }
            }

            std::pair<KeyType,char> Get()
            {
                int ch = std::getchar();
                switch( ch )
                {
                    case EOF:
                    case 4:  // EOT
                        return std::make_pair(KeyType::eof,' ');
                        break;
                    case 127: return std::make_pair(KeyType::backspace,' '); break;
                    case 10: return std::make_pair(KeyType::ret,' '); break;
                    case 27: // symbol
                        ch = std::getchar();
                        if ( ch == 91 ) // arrow keys
                        {
                            ch = std::getchar();
                            switch( ch )
                            {
                                case 51:
                                    ch = std::getchar();
                                    if ( ch == 126 ) return std::make_pair(KeyType::canc,' ');
                                    else return std::make_pair(KeyType::ignored,' ');
                                    break;
                                case 65: return std::make_pair(KeyType::up,' ');
                                case 66: return std::make_pair(KeyType::down,' ');
                                case 68: return std::make_pair(KeyType::left,' ');
                                case 67: return std::make_pair(KeyType::right,' ');
                                case 70: return std::make_pair(KeyType::end,' ');
                                case 72: return std::make_pair(KeyType::home,' ');
                                default: return std::make_pair(KeyType::ignored,' ');
                            }
                        }
                        break;
                    default: // ascii
                    {
                        const char c = static_cast<char>(ch);
                        return std::make_pair(KeyType::ascii,c);
                    }
                }
                return std::make_pair(KeyType::ignored,' ');
            }

            void ToManualMode()
            {
                constexpr tcflag_t ICANON_FLAG = ICANON;
                constexpr tcflag_t ECHO_FLAG = ECHO;

                tcgetattr( STDIN_FILENO, &oldt );
                newt = oldt;
                newt.c_lflag &= ~( ICANON_FLAG | ECHO_FLAG );
                tcsetattr( STDIN_FILENO, TCSANOW, &newt );
            }

            void ToStandardMode()
            {
                tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
            }

            termios oldt;
            termios newt;
            std::atomic<bool> run { true };
            std::unique_ptr<std::thread> servant;
        };
    } // namespace detail
} // namespace cli

#endif // CLI_DETAIL_LINUXKEYBOARD_H_
