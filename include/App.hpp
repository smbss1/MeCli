#ifndef MECLI_APP_HPP_
#define MECLI_APP_HPP_

#include <functional>
#include "Common.hpp"
#include "LoopScheduler.hpp"
#include "LinuxKeyboard.hpp"
#include "InputDevice.hpp"
#include "Terminal.hpp"
#include "History.hpp"
#include "Trie.hpp"

namespace mecli
{
    class App
    {
    public:
        App() : m_oKeyboard(m_oScheduler), terminal(std::cout, m_vSyntaxes)
        {
            m_oLex.m_bEnableError = false;
            m_oLex.Define("WS", "[ \t\r\b]+", true);
            m_oLex.Define("NewLine", "\n", true);
            m_oLex.Define("Number", "[0-9]+");
            m_oLex.Define("Command", "/[A-Za-z_]+");
            m_oLex.Define("Identifier", "[A-Za-z_]+");
            m_oLex.Define("Eof", "\\0", true);

            m_oKeyboard.Register([this](auto key)
            {
                this->OnKeyPressed(key);
            });

            m_bExit = false;
            AddCmd("exit", [this](ArgsList&) { Exit(); });
            AddCmd("history", [this](ArgsList&) { m_oHistory.Dump(std::cout); });
            AddCmd("clear", [this](ArgsList& vArgs)
            {
                if (vArgs[1] == "history")
                    m_oHistory.Clear();
            });

            AddCmd("help", [this](ArgsList& vArgs)
            {
                PrintHelp();
            });
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

        void AddCmd(const std::string& strCmd, const std::string& strHelp, CmdFunc oFunc)
        {
            m_vCmds.push_back(Command("/" + strCmd, strHelp, oFunc));
            AddSyntax("/" + strCmd, [](const std::string& strText)
            {
                std::cout << style::bold << style::blue << strText << style::reset;
            });
        }

        void SetOnLineListener(OnLineFunc oFunc)
        {
            m_oOnLineListener = oFunc;
        }

        void AddCmd(const std::string& strCmd, CmdFunc oFunc)
        {
            AddCmd(strCmd, "", oFunc);
        }

        void AddSyntax(const std::string& strSyntax, SyntaxFunc oFunc)
        {
            m_vSyntaxes.emplace(strSyntax, oFunc);
            m_oTrie.Insert(strSyntax);
        }

        void OnKeyPressed(std::pair<detail::KeyType, char> k)
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

        void PrintHelp()
        {
            std::cout << "Commands available: " << std::endl;
            for (auto& cmd : m_vCmds)
            {
                std::cout << "  - " << cmd.GetName() << std::endl;
                std::cout << "            " << cmd.GetDesc() << std::endl;
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
                    // Parse the line
                    m_oLex.Process(s.second);
                    m_oLex.Begin();
                    bool bFound = false;
                    for (int i = 0; i < m_vCmds.size(); ++i)
                    {
                        // If found the correct command execute it function
                        if (m_vCmds[i].GetName() == m_oLex[0].GetText())
                        {
                            bFound = true;
                            m_vCmds[i].Exec(m_oLex.m_strList);
                            break;
                        }
                    }
                    if (!bFound && m_oOnLineListener)
                        m_oOnLineListener(s.second);
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
                case detail::Symbol::tab:
                {
                    auto line = terminal.GetLine();
                    m_oLex.Process(line);
                    m_oTrie.IsSpace(m_oLex[m_oLex.Size() - 1].GetText());
                    std::vector<std::string> vCompletions = m_oTrie.Search(m_oLex[m_oLex.Size() - 1].GetText());
                    if (!vCompletions.empty())
                    {
                        std::cout << std::endl;
                        std::string items;
                        std::for_each(vCompletions.begin(), vCompletions.end(), [&items](auto& cmd)
                        {
                            items += "    " + cmd;
                        });
                        std::cout << items << std::endl;
                        Prompt();
                        terminal.ResetCursor();
                        terminal.SetLine( line );
                    }
                    break;
                }
            }
        }

    private:
        LoopScheduler m_oScheduler;
        detail::LinuxKeyboard m_oKeyboard;
        CmdList m_vCmds;
        SyntaxList m_vSyntaxes;
        Lexer m_oLex;
        detail::Terminal terminal;
        bool m_bExit;
        History m_oHistory;
        Trie m_oTrie;
        OnLineFunc m_oOnLineListener;
    };
}

#endif