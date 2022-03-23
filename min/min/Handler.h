#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iostream>

struct Pair
{
	std::string q, y;
};

typedef std::vector<std::vector<Pair>> MachineType;

const std::string ML_STRING = "Ml";
const std::string MR_STRING = "Mr";
const std::string MACHINE_TYPE_ERROR = "Machine type error";

class Handler final
{
public:
	Handler();

	void Run();

private:
	void ReadMealyMachine();
	void PrintMealyMachine() const;
	void ReadMooreMachine();
	void PrintMooreMachine() const;
	void Minimize();

private:
	MachineType m_machine;
	MachineType m_minMachine;
	std::vector<std::string> m_Q;
	std::vector<std::string> m_minQ;
	std::vector<std::string> m_X;
	std::map<std::string, std::vector<std::string>> m_statesByState;
	std::map<std::string, std::vector<std::string>> m_yByState;
};
