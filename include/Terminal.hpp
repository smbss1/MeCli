#ifndef CLI_DETAIL_TERMINAL_HPP_
#define CLI_DETAIL_TERMINAL_HPP_


#include <iostream>
#include <algorithm>

#include "Common.hpp"
#include "InputDevice.hpp"
#include "Lexer.h"

namespace cli
{
    namespace detail
    {
        enum class Symbol
        {
            nothing,
            command,
            up,
            down,
            tab,
            eof
        };

        class Terminal
        {
        public:
            Terminal(Lexer& lex, std::ostream &_out, CmdList& vCmds) : m_vCmds(vCmds), out(_out), m_oLex(lex)
            {
                // m_strCurrentLine += "> ";
                m_lMinPosition = 0;
                m_lPosition = m_lMinPosition;

                m_vCmds = vCmds;
            }

            void ResetCursor() { m_lPosition = m_lMinPosition; }

            void SetLine(const std::string &newLine)
            {
                out << std::string(m_lPosition, '\b') << newLine << std::flush;

                // if newLine is shorter then currentLine, we have
                // to clear the rest of the string
                if (newLine.size() < m_strCurrentLine.size())
                {
                    out << std::string(m_strCurrentLine.size() - newLine.size(), ' ');
                    // and go back
                    out << std::string(m_strCurrentLine.size() - newLine.size(), '\b') << std::flush;
                }

                m_strCurrentLine = newLine;
                m_lPosition = m_strCurrentLine.size();
            }

            std::string GetLine() const { return m_strCurrentLine; }

            std::pair<Symbol, std::string> Keypressed(std::pair<KeyType, char> k)
            {
                switch (k.first)
                {
                    case KeyType::eof:
                        return std::make_pair(Symbol::eof, std::string{});
                        break;
                    case KeyType::backspace:
                    {
                        if (m_lPosition == m_lMinPosition)
                            break;

                        --m_lPosition;

                        const auto pos = static_cast<std::string::difference_type>(m_lPosition);
                        // remove the char from buffer
                        m_strCurrentLine.erase(m_strCurrentLine.begin() + pos);
                        // go back to the previous char
                        out << '\b';
                        // output the rest of the line
                        out << std::string(m_strCurrentLine.begin() + pos, m_strCurrentLine.end());
                        // remove last char
                        out << ' ';
                        // go back to the original position
                        out << std::string(m_strCurrentLine.size() - m_lPosition + 1, '\b') << std::flush;
                        break;
                    }
                    case KeyType::up:
                        return std::make_pair(Symbol::up, std::string{});
                        break;
                    case KeyType::down:
                        return std::make_pair(Symbol::down, std::string{});
                        break;
                    case KeyType::left:
                        if (m_lPosition > m_lMinPosition)
                        {
                            out << '\b' << std::flush;
                            --m_lPosition;
                        }
                        break;
                    case KeyType::right:
                        if (m_lPosition < m_strCurrentLine.size())
                        {
                            out << m_strCurrentLine[m_lPosition] << std::flush;
                            ++m_lPosition;
                        }
                        break;
                    case KeyType::ret:
                    {
                        out << "\n";
                        auto cmd = m_strCurrentLine;
                        m_strCurrentLine.clear();
                        m_lPosition = m_lMinPosition;
                        return std::make_pair(Symbol::command, cmd);
                    }
                    case KeyType::ascii:
                    {
                        const char c = static_cast<char>(k.second);
                        if (c == '\t')
                            return std::make_pair(Symbol::tab, std::string());
                        else
                        {
                            const auto pos = static_cast<std::string::difference_type>(m_lPosition);

                            // clear the line
                            for (int i = 0; i < pos; ++i)
                                out << "\b";

                            // update the buffer and cursor position:
                            m_strCurrentLine.insert(m_strCurrentLine.begin() + pos, c);
                            ++m_lPosition;

                            // Parse
                            m_oLex.Process(m_strCurrentLine);
                            m_oLex.Begin();
                            bool bFound = false;
                            for (int i = 0; i < m_vCmds.size(); ++i)
                            {
                                // If the first command is correct so highlight it
                                if (m_vCmds[i].GetName() == m_oLex[0].GetText())
                                {
                                    bFound = true;
                                    out << "\033[1;32m" << m_oLex[0].GetText() << "\033[0;0m" << std::flush;
                                    break;
                                }
                            }
                            for (int i = 0; i < m_oLex.oTokenList.size(); ++i)
                            {
                                auto& oToken = m_oLex[i];
                                if (oToken.IsError() || (bFound && i == 0))
                                    continue;
                                out << oToken.GetText() << std::flush;
                            }
                            out << std::string(m_strCurrentLine.size() - m_lPosition, '\b') << std::flush;
                        }
                        break;
                    }
                    case KeyType::canc:
                    {
                        if (m_lPosition == m_strCurrentLine.size())
                            break;
                    
                        const auto pos = static_cast<std::string::difference_type>(m_lPosition);

                        // output the rest of the line
                        out << std::string(m_strCurrentLine.begin() + pos + 1, m_strCurrentLine.end());
                        // remove last char
                        out << ' ';
                        // go back to the original position
                        out << std::string(m_strCurrentLine.size() - m_lPosition + 2, '\b') << std::flush;
                        // remove the char from buffer
                        m_strCurrentLine.erase(m_strCurrentLine.begin() + pos);
                        break;
                    }
                    case KeyType::end:
                    {
                        const auto pos = static_cast<std::string::difference_type>(m_lPosition);

                        out << std::string(m_strCurrentLine.begin() + pos, m_strCurrentLine.end()) << std::flush;
                        m_lPosition = m_strCurrentLine.size();
                        break;
                    }
                    case KeyType::home:
                    {
                        out << std::string(m_lPosition, '\b') << std::flush;
                        m_lPosition = m_lMinPosition;
                        break;
                    }
                    case KeyType::ignored:
                        // TODO
                        break;
                }

                return std::make_pair(Symbol::nothing, std::string());
            }

        private:
            std::string m_strCurrentLine;
            std::size_t m_lPosition; // next writing position in currentLine
            std::size_t m_lMinPosition;
            std::ostream &out;
            CmdList& m_vCmds;
            Lexer& m_oLex;
        };
    }
}

#endif