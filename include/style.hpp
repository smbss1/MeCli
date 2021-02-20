
#ifndef CLI_STYLE_HPP_
#define CLI_STYLE_HPP_

#include <iostream>

namespace cli::style
{
    static int id = 0;
    static int bg = 0;
    #define SET_COLOR(c) "\033[" << id << ";" << 30 + bg + c << "m"

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& standard(std::basic_ostream<CharT, Traits>& os)
    {
        id = 0;
        return os;
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& bold(std::basic_ostream<CharT, Traits>& os)
    {
        id = 1;
        return os;
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& underline(std::basic_ostream<CharT, Traits>& os)
    {
        id = 4;
        return os;
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& background(std::basic_ostream<CharT, Traits>& os)
    {
        bg = 10;
        return os;
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& reset(std::basic_ostream<CharT, Traits>& os)
    {
        bg = 0;
        id = 0;
        return os << "\033[0m";
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& black(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(0);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& red(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(1);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& green(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(2);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& yellow(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(3);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& blue(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(4);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& purple(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(5);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& cyan(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(6);
    }

    template <class CharT, class Traits>
    constexpr
    std::basic_ostream<CharT, Traits>& white(std::basic_ostream<CharT, Traits>& os)
    {
        return os << SET_COLOR(7);
    }
}

#endif