#include "Handler.h"

Handler::Handler()
{
}

void Handler::Run()
{
	std::string str;
	getline(std::cin, str);
	if (str == ML_STRING)
	{
		ReadMealyMachine();
		Minimize();
		PrintMealyMachine();
	}
	else if (str == MR_STRING)
	{
		ReadMooreMachine();
		Minimize();
		PrintMooreMachine();
	}
	else
	{
		std::cout << MACHINE_TYPE_ERROR << std::endl;
	}
}

void Handler::Minimize()
{
	std::set<std::vector<std::string>> groupsByY;
	std::vector<std::vector<std::string>> currQBygroups;
	
	//grouping by y
	for (int i = 0; i < m_Q.size(); i++)
	{
		if (groupsByY.find(m_yByState[m_Q[i]]) == groupsByY.end())
		{
			groupsByY.insert(m_yByState[m_Q[i]]);
			currQBygroups.push_back({ m_Q[i] });
		}
		else
		{
			int k = 0;
			for (auto& const el : currQBygroups)
			{
				if (m_yByState[m_Q[i]] == m_yByState[el[0]])
				{
					currQBygroups[k].push_back(m_Q[i]);
				}
				k++;
			}
		}
	}

	//to get group number by q
	std::map<std::string, int> groupsByQ;
	std::vector<std::string> newQ;
	int k = 0;
	for (auto& const v : currQBygroups)
	{
		for (auto& const el : v)
		{
			groupsByQ[el] = k;
			newQ.push_back(el);
		}
		k++;
	}

	//main cycle
	std::vector<std::vector<std::string>> prevQBygroups;
	while (currQBygroups.size() != prevQBygroups.size() and currQBygroups.size() < m_Q.size())
	{
		prevQBygroups = currQBygroups;
		currQBygroups.clear();

		for (int i = 0; i < newQ.size(); i++)
		{
			if (currQBygroups.empty())
			{
				currQBygroups.push_back({ newQ[i] });
			}
			else
			{
				bool equal = true;
				for (int j = 0; j < m_X.size(); j++)
				{
					//comparing to prev column by q
					equal = equal and (groupsByQ[m_statesByState[newQ[i - 1]][j]] == groupsByQ[m_statesByState[newQ[i]][j]]);
				}

				if (equal)
				{
					currQBygroups[currQBygroups.size() - 1].push_back(newQ[i]);
				}
				else
				{
					currQBygroups.push_back({ newQ[i] });
				}
			}
		}

		groupsByQ.clear();
		k = 0;
		for (auto& const v : currQBygroups)
		{
			for (auto& const el : v)
			{
				groupsByQ[el] = k;
			}
			k++;
		}
	}

	//new states
	for (int i = 0; i < currQBygroups.size(); i++)
	{
		m_minQ.push_back("S" + std::to_string(i + 1));
	}

	for (int i = 0; i < m_X.size(); i++)
	{
		m_minMachine.push_back({});
		for (int j = 0; j < currQBygroups.size(); j++)
		{
			std::string q = currQBygroups[j][0];
			m_minMachine[i].push_back({ "S" + std::to_string(groupsByQ[m_statesByState[q][i]] + 1),  m_yByState[q][i] });
		}
	}	
}

void Handler::ReadMealyMachine()
{
	std::string str;
	getline(std::cin, str);
	std::stringstream ss;
	ss << str;

	while (ss >> str)
	{
		m_Q.push_back(str);
		m_statesByState[str] = {};
		m_yByState[str] = {};
	}

	ss.clear();
	while (getline(std::cin, str))
	{
		ss << str;
		std::string x;
		ss >> x;
		m_X.push_back(x);
		m_machine.resize(m_X.size());

		int i = 0;
		while (ss >> str)
		{
			std::string y = str.substr(str.find("/") + 1);
			std::string q = str.substr(0, str.length() - y.length() - 1);
			m_machine[m_X.size() - 1].push_back({q, y});
			m_statesByState[m_Q[i]].push_back(q);
			m_yByState[m_Q[i]].push_back(y);
			i++;
		}
		ss.clear();
	}
}

void Handler::ReadMooreMachine()
{
	std::string str;
	std::stringstream ss;

	getline(std::cin, str);
	ss << str;
	std::vector<std::string> y;
	while (ss >> str)
	{
		y.push_back(str);
	}

	getline(std::cin, str);
	ss.clear();
	ss << str;
	while (ss >> str)
	{
		m_Q.push_back(str);
		m_statesByState[str] = {};
		m_yByState[str] = {};
	}

	ss.clear();
	while (getline(std::cin, str))
	{
		ss << str;
		std::string x;
		ss >> x;
		m_X.push_back(x);
		m_machine.resize(m_X.size());

		int count = 0;
		while (ss >> str)
		{
			m_machine[m_X.size() - 1].push_back({ str, y[count] });
			m_statesByState[m_Q[count]].push_back(str);
			m_yByState[m_Q[count]].push_back(y[count]);
			count++;
		}
		ss.clear();
	}
}

void Handler::PrintMealyMachine() const
{
	std::cout << ML_STRING << std::endl;

	for (const auto& elem : m_minQ)
	{
		std::cout << "\t" << elem;
	}
	std::cout << std::endl;

	for (int i = 0; i < m_X.size(); i++)
	{
		std::cout << m_X[i];
		for (const auto& elem : m_minMachine[i])
		{
			std::cout << "\t" << elem.q << "/" << elem.y;
		}
		std::cout << std::endl;
	}
}

void Handler::PrintMooreMachine() const
{
	std::cout << ML_STRING << std::endl;

	for (const auto& elem : m_minMachine[0])
	{
		std::cout << "\t" << elem.y;
	}
	std::cout << std::endl;

	for (const auto& elem : m_minQ)
	{
		std::cout << "\t" << elem;
	}
	std::cout << std::endl;

	for (int i = 0; i < m_X.size(); i++)
	{
		std::cout << m_X[i];
		for (const auto& elem : m_minMachine[i])
		{
			std::cout << "\t" << elem.q;
		}
		std::cout << std::endl;
	}
}