#ifndef CLI_APP_HPP_
#define CLI_APP_HPP_

#include "Common.hpp"
#include "LoopScheduler.hpp"
#include "LinuxKeyboard.hpp"
#include "InputDevice.hpp"
#include "Terminal.hpp"
#include "History.hpp"

namespace cli
{
    class App
    {
    public:
        App() : m_oKeyboard(m_oScheduler), terminal(lex, std::cout, m_vCmds)
        {
            lex.m_bEnableError = false;
            lex.Define("WS", "[ \t\r\b]+");
            lex.Define("NewLine", "\n");
            lex.Define("Number", "[0-9]+");
            lex.Define("Identifier", "[A-Za-z_]+[0-9]*");
            lex.Define("Eof", "\\0");

            m_oKeyboard.Register([this](auto key)
            {
                this->Keypressed(key);
            });

            m_bExit = false;
            AddCmd("exit", [this]() { Exit(); });
        }

        ~App() { }

        void Run()
        {
            Prompt();
            m_oScheduler.Run();
        }

        void Exit()
        {
            std::cout << std::endl;
            std::cout << "Bye!! Bye!!" << std::endl;
            m_bExit = true;
            m_oScheduler.Stop();
        }

        void AddCmd(const std::string& strCmd, CmdFunc oFunc)
        {
            m_vCmds.push_back(Command(strCmd, oFunc));
        }

        void Keypressed(std::pair<detail::KeyType, char> k)
        {
            const std::pair<detail::Symbol, std::string> s = terminal.Keypressed(k);
            NewCommand(s);
        }

        void Prompt()
        {
            if (!m_bExit)
            {
                std::cout << style::bold << style::white;
                std::cout << "> " << style::reset << std::flush;
            }
        }

        void NewCommand(const std::pair<detail::Symbol, std::string>& s)
        {
            switch (s.first)
            {
                case detail::Symbol::nothing:
                    break;
                case detail::Symbol::eof:
                {
                    Exit();
                    break;
                }
                case detail::Symbol::command:
                {
                    for (int i = 0; i < m_vCmds.size(); ++i)
                    {
                        // If found the correct command execute it function
                        if (m_vCmds[i].GetName() == lex[0].GetText())
                        {
                            m_vCmds[i].Exec();
                            break;
                        }
                    }
                    if (!s.second.empty())
                        m_oHistory.Add(s.second);
                    Prompt();
                    break;
                }
                case detail::Symbol::down:
                {
                    terminal.SetLine(m_oHistory.Next());
                    break;
                }
                case detail::Symbol::up:
                {
                    terminal.SetLine(m_oHistory.Prev());
                    break;
                }
            }

        }

    private:
        cli::LoopScheduler m_oScheduler;
        cli::detail::LinuxKeyboard m_oKeyboard;
        CmdList m_vCmds;
        Lexer lex;
        detail::Terminal terminal;
        bool m_bExit;
        History m_oHistory;
    };
}

#endif