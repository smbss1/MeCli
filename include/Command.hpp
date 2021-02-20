#ifndef CLI_COMMAND_HPP_
#define CLI_COMMAND_HPP_

#include <string>
#include <functional>

namespace cli
{
    using CmdFunc = std::function<void()>;
}

namespace cli
{
    class Command
    {
    public:
        Command(const std::string& strName, CmdFunc oFunc)
        {
            m_strName = strName;
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

        void Exec()
        {
            m_oFunc();
        }

    private:
        std::string m_strName;
        CmdFunc m_oFunc;
    };
}

#endif