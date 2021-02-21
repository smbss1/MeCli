

#ifndef MECLI_HISTORY_STORAGE_HPP_
#define MECLI_HISTORY_STORAGE_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class History
{
public:
    History()
    {
        Load("history.txt");
        Dump(std::cout);
        m_lCurrentLine = m_vHistory.size();
    }

    ~History()
    {
        Save("history.txt");
    }

    void Add(const std::string& strLine)
    {
        if (m_vHistory.empty() || m_vHistory[m_vHistory.size() - 1] != strLine) // insert an element not equal to last one
            m_vHistory.push_back(strLine);
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
        std::ofstream out(strFilename);
        if (out)
        {
            for (auto& cmd : m_vHistory)
                out << cmd << '\n';
            out << std::flush;
        }
    }
private:
    std::vector<std::string> m_vHistory;
    std::size_t m_lCurrentLine;
};

#endif