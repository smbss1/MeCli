

#ifndef MECLI_HISTORY_STORAGE_HPP_
#define MECLI_HISTORY_STORAGE_HPP_

#include <string>
#include <deque>
#include <iostream>
#include <fstream>

namespace mecli
{
    class History
    {
    public:
        History()
        {
            Load("history.txt");
            m_lCurrentLine = m_vHistory.size();
            m_lMaxSize = 200;
        }

        ~History()
        {
            Save("history.txt");
        }

        void Add(const std::string& strLine)
        {
            if (m_vHistory.size() > m_lMaxSize)
                m_vHistory.erase(m_vHistory.begin());
            if (m_vHistory.empty() || m_vHistory[m_vHistory.size() - 1] != strLine) // insert an element not equal to last one
                m_vHistory.push_back(strLine);
        }

        void Clear()
        {
            std::cout << "Clear" << std::endl;
            m_vHistory.clear();
        }

        void Dump(std::ostream& out)
        {
            for (auto& cmd : m_vHistory)
                out << cmd << '\n';
            out << '\n' << std::flush;
        }

        const std::string& Next()
        {
            if (m_lCurrentLine < m_vHistory.size() - 1)
                return m_vHistory[++m_lCurrentLine];
            return m_vHistory[m_lCurrentLine];
        }

        const std::string& Prev()
        {
            if (m_lCurrentLine > 0)
                return m_vHistory[--m_lCurrentLine];
            return m_vHistory[m_lCurrentLine];
        }

    private:
        void Load(const std::string& strFilename)
        {
            std::ifstream in(strFilename);
            if (in)
            {
                std::string strLine;
                m_vHistory.clear();
                while (std::getline(in, strLine))
                    m_vHistory.push_back(strLine);
            }
        }

        void Save(const std::string& strFilename)
        {
            std::ofstream out(strFilename, std::ofstream::out | std::ofstream::trunc);
            if (out)
            {
                if (m_vHistory.empty())
                    out.clear();
                for (auto& cmd : m_vHistory)
                    out << cmd << '\n';
                out << std::flush;
            }
        }
    private:
        std::vector<std::string> m_vHistory;
        std::size_t m_lCurrentLine;
        std::size_t m_lMaxSize;
    };
} // namespace mecli

#endif