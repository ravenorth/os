#include "Handler.h"

Handler::Handler(std::istream& inputFile, std::ostream& outputFile)
	: m_inputFile(inputFile)
	, m_outputFile(outputFile)
{
}

void Handler::Run()
{
	std::string str;
	getline(m_inputFile, str);
	if (str == L_STRING)
	{
		ReadLeftGrammar();
		Determinate();
		PrintMachine();
	}
	else if (str == R_STRING)
	{
		ReadRightGrammar();
		Determinate();
		PrintMachine();
	}
	else
	{
		m_outputFile << GRAMMAR_TYPE_ERROR << std::endl;
	}
}

void Handler::CheckQ(std::string s)
{
	if (!(s.empty()) and (m_detQ.find(s) == m_detQ.end()))
	{
		m_detQ.insert(s);
		m_detMachine[s] = m_machine[s];
	}

	for (auto& const q : m_machine[s])
	{
		if (!(q.empty()) and (m_detQ.find(q) == m_detQ.end()))
		{
			m_detQ.insert(q);
			m_detMachine[q] = m_machine[q];
			if (q != s)
				CheckQ(q);
		}
	}
}

void Handler::Determinate()
{
	//sort states
	for (auto& pair : m_machine)
	{
		for (auto& str : pair.second)
		{
			if (!str.empty())
			{
				sort(str.begin(), str.end());
			}
		}
	}

	//fill all complex states
	for (auto& const pair : m_machine)
	{
		for (int i = 0; i < m_X.size(); i++)
		{
			if (!(pair.second[i].empty()) and (m_Q.find(pair.second[i]) == m_Q.end()))
			{
				m_Q.insert(pair.second[i]);
				m_machine[pair.second[i]] = std::vector<std::string>(m_X.size(), "");

				for (int j = 0; j < m_X.size(); j++)
				{
					std::set<std::string> temp;
					for (char& const ch1 : pair.second[i])
					{
						//to avoid states repeat
						for (char& const ch2 : m_machine[std::string(1, ch1)][j])
						{
							if (temp.find(std::string(1, ch2)) == temp.end())
							{
								temp.insert(std::string(1, ch2));
							}
						}
					}

					for (auto& const el : temp)
					{
						m_machine[pair.second[i]][j] += el;
					}
				}
			}
		}
	}

	//check peaks reachability
	CheckQ(m_firstQ);
}

void Handler::ReadLeftGrammar()
{
	std::string str;

	getline(m_inputFile, str);
	std::stringstream ss;
	ss << str;
	while (ss >> str)
	{
		m_X.push_back(str);
	}

	m_firstQ = "H";
	while (getline(m_inputFile, str))
	{
		std::string qTo = std::string(1, str[0]);
		for (int i = 1; i < str.length(); i++)
		{
			std::string qFrom = "H";
			std::string x;
			if (str[i] == '|' or str[i] == '-')
			{
				if (str[i + 1] >= 'A' and str[i + 1] <= 'Z')
				{
					qFrom = std::string(1, str[i + 1]);
					x = std::string(1, str[i + 2]);
				}
				else
				{
					x = std::string(1, str[i + 1]);
				}

				if (m_Q.find(qFrom) == m_Q.end())
				{
					m_Q.insert(qFrom);
					m_machine[qFrom] = {"", "", ""};
				}

				int index = std::find(m_X.begin(), m_X.end(), x) - m_X.begin();
				m_machine[qFrom][index] += qTo;
			}
		}
	}

	if (m_Q.find("S") == m_Q.end())
	{
		m_Q.insert("S");
		m_machine["S"] = std::vector<std::string>(m_X.size(), "");
	}
}

void Handler::ReadRightGrammar()
{
	std::string str;

	getline(m_inputFile, str);
	std::stringstream ss;
	ss << str;
	while (ss >> str)
	{
		m_X.push_back(str);
	}

	m_firstQ = "S";
	while (getline(m_inputFile, str))
	{
		std::string qFrom = std::string(1, str[0]);
		if (m_Q.find(qFrom) == m_Q.end())
		{
			m_Q.insert(qFrom);
			m_machine[qFrom] = std::vector<std::string>(m_X.size(), "");
		}
		for (int i = 1; i < str.length(); i++)
		{
			std::string qTo = "H";
			std::string x;
			if (str[i] == '|' or str[i] == '-')
			{
				if (str[i + 2] >= 'A' and str[i + 2] <= 'Z')
				{
					qTo = std::string(1, str[i + 2]);
					x = std::string(1, str[i + 1]);
				}
				else
				{
					x = std::string(1, str[i + 1]);
				}

				int index = std::find(m_X.begin(), m_X.end(), x) - m_X.begin();
				m_machine[qFrom][index] += qTo;
			}
		}
	}

	if (m_Q.find("H") == m_Q.end())
	{
		m_Q.insert("H");
		m_machine["H"] = std::vector<std::string>(m_X.size(), "");
	}
}

void Handler::PrintMachine() const
{
	for (const auto& elem : m_X)
	{
		m_outputFile << "\t" << elem;
	}
	m_outputFile << std::endl;

	for (const auto& pair : m_detMachine)
	{
		m_outputFile << pair.first;
		for (const auto& elem : pair.second)
		{
			std::string res = (elem.empty()) ? "-" : elem;
			m_outputFile << "\t" << res;
		}
		m_outputFile << std::endl;
	}
}
