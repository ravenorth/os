#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

const std::string L_STRING = "L";
const std::string R_STRING = "R";
const std::string GRAMMAR_TYPE_ERROR = "Grammar type error";

const std::set<char> validQ = {'A', 'B', 'C', 'D', 'E'};

class Handler final
{
public:
	Handler(std::istream& inputFile, std::ostream& outputFile);

	void Run();

private:
	void ReadLeftGrammar();
	void ReadRightGrammar();
	void PrintMachine() const;
	void Determinate();
	void CheckQ(std::string s);

private:
	std::istream& m_inputFile;
	std::ostream& m_outputFile;
	std::string m_firstQ;
	std::vector<std::string> m_X;
	std::set<std::string> m_Q;
	std::set<std::string> m_detQ;
	std::map<std::string, std::vector<std::string>> m_machine;
	std::map<std::string, std::vector<std::string>> m_detMachine;

};
