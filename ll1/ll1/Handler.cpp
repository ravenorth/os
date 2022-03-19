#include "Handler.h"

Handler::Handler(std::ifstream& tableFile)
    : m_tableFile(tableFile)
{
    ReadTable();
}

void Handler::GC()
{
    if (!m_list.empty())
    {
        m_ch = m_list.front();
        m_list.pop_front();
    }
    else
    {
        m_ch = EMPTY_SYMBOL;
    }
}

bool Handler::Run(std::string str)
{
    for (char& c : str)
    {
        m_list.push_back(c);
    }

    GC();

    int currRow = 0;
    while (!m_table[currRow].end)
    {
        if (m_table[currRow].symbol >= 'A' and m_table[currRow].symbol <= 'Z')
        {
            if (m_table[currRow].guideSymbols.find(m_ch) == m_table[currRow].guideSymbols.end())
            {
                if (m_table[currRow].error)
                {
                    return false;
                }

                currRow++;
            }
            else
            {
                if (m_table[currRow].stack)
                {
                    m_stack.push(currRow + 1);
                }

                currRow = m_table[currRow].pointer;
            }
        }
        else
        {
            if (m_ch == m_table[currRow].symbol)
            {
                if (m_table[currRow].shift)
                {
                    GC();
                }

                if (m_table[currRow].pointer == NULL_POINTER)
                {
                    currRow = m_stack.top();
                    m_stack.pop();
                }
                else
                {
                    currRow = m_table[currRow].pointer;
                }
            }
            else
            {
                return false;
            }
        }
    }

    return (m_ch == EMPTY_SYMBOL);
}

void Handler::ReadTable()
{
    std::string line;

    while (getline(m_tableFile, line))
    {
        std::stringstream ss(line);
        Row row;
        std::string symbols, symbol;
        int x;

        ss >> x
            >> symbol
            >> symbols
            >> row.shift
            >> row.error
            >> row.pointer
            >> row.stack
            >> row.end;

        row.symbol = symbol[0];
        for (char& ch : symbols)
        {
            row.guideSymbols.insert(ch);
        }

        m_table.push_back(row);
    }
}