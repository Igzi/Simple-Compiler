#ifndef machine_h
#define machine_h

#include "Variable.h"
#include "Command.h"

#include <vector>
#include <stack>

class Machine {
public:
	Machine();
	~Machine();

	void loadProgram(const string& filepath);
	void execute(const string& filepath);

private:
	bool isVariable(string& name);
	bool isInt(string& name);
	Variable* makeVariable(string& name);
	Variable* getVariable(string& name);

	void loadInstruction(string& instruction);
	
	void clearMachine();

	vector<Variable*> variables, constants;
	vector<Command*> instructions;
	stack<pair<int, string>> instruction_stack;
	int pc;
};

#endif // machine_h
