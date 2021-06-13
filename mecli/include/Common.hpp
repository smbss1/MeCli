
#ifndef MECLI_COMMON_HPP_
#define MECLI_COMMON_HPP_

#include <vector>
#include <string>
#include <unordered_map>
#include "Command.hpp"

namespace mecli
{
    using CmdList = std::vector<Command>;
    using SyntaxFunc = std::function<void(const std::string&)>;
    using SyntaxList = std::unordered_map<std::string, SyntaxFunc>;
    using OnLineFunc = std::function<void(const std::string&)>;
}

#endif