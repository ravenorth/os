#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <sstream>
#include <deque>

const char EMPTY_SYMBOL = '@';
const int NULL_POINTER = -1;

struct Row
{
	char symbol;
	std::set<char> guideSymbols;
	bool shift;
	bool error;
	int pointer;
	bool stack;
	bool end;
};

class Handler
{
public:
	Handler(std::ifstream& tableFile);

	bool Run(std::string str);

private:
	void ReadTable();
	void GC();

private:
	std::ifstream& m_tableFile;
	std::vector<Row> m_table;
	std::stack<int> m_stack;
	std::deque<char> m_list;
	char m_ch;
};

