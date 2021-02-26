#ifndef MECLI_COMMAND_HPP_
#define MECLI_COMMAND_HPP_

#include <string>
#include <vector>
#include <functional>

namespace mecli
{
    using ArgsList = const std::vector<std::string>;
    using CmdFunc = std::function<void(ArgsList&)>;
}

namespace mecli
{
    class Command
    {
    public:
        Command(const std::string& strName, const std::string& strDesc, CmdFunc oFunc)
        {
            m_strName = strName;
            m_strDesc = strDesc;
            m_oFunc = oFunc;
        }
        ~Command() { }

        bool operator==(const Command& other) const
        {
            return m_strName == other.m_strName;
        }

        const std::string& GetName() const
        {
            return m_strName;
        }

        const std::string& GetDesc() const
        {
            return m_strDesc;
        }

        void Exec(ArgsList& vArgs)
        {
            m_oFunc(vArgs);
        }

    private:
        std::string m_strName;
        std::string m_strDesc;
        CmdFunc m_oFunc;
    };
}

#endif