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
	void checkGoTo();
	bool checkVariable(Variable* a);

	void loadSet(Variable* a, Variable* b);
	void loadAdd(Variable* a, Variable* b, Variable* c);
	void loadSub(Variable* a, Variable* b, Variable* c);
	void loadMul(Variable* a, Variable* b, Variable* c);
	void loadDiv(Variable* a, Variable* b, Variable* c);
	void loadGoTo(Variable* a);
	void loadIfGr(Variable* a, Variable* b);
	void loadIfEq(Variable* a, Variable* b);
	void loadElse();
	void loadEndIf();
	void loadLoop(Variable* a);
	void loadEndLoop();
	
	void clearMachine();

	vector<Variable*> variables, constants; //skup svih variabli i konstanti
	vector<Command*> instructions; //skup zadatih instrukcija
	vector<pair<int, int>> variable_scope; //intervali instrukcija na kojima neka variabla postoji
	stack<pair<int, Variable*>> active_variables; //skup variabli koje su trenutno inicijalizovane
	stack<pair<int, string>> instruction_stack;
	int pc;
};

#endif // machine_h
