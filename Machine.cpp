#include "Machine.h"
#include "Exceptions.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

//Maksimalan broj reci u jednoj instrukciji
const int MAXINSTRUCTIONSIZE = 4;

Machine::Machine()
{
	pc = 0;
}

Machine::~Machine()
{
	clearMachine();
}

void Machine::loadProgram(const string& filepath)
{
	clearMachine();
	ifstream input(filepath);
	string instruction, name_a, name_b, name_c;
	Variable* a, * b, * c;
	a = b = c = nullptr;
	int move = 0;

	while (getline(input,instruction)) {
		try {
			loadInstruction(instruction);
		}
		catch (SyntaxError& e){
			cout << "Syntax error: line " << e.getLine() << ", " << e.what() << endl;
			clearMachine();
			return;
		}
	}

	try {
		checkGoTo();
	}
	catch (SyntaxError& e) {
		cout << "Syntax error: line " << e.getLine() << ", " << e.what() << endl;
		clearMachine();
		return;
	}

	if (instruction_stack.size()) {
		cout << "Syntax error: Missing ENDLOOP or ENDIF instructions"<< endl;
		clearMachine();
		return;
	}

	while (active_variables.size()) {
		active_variables.pop();
	}
	variable_scope.clear();
}

void Machine::execute(const string& filepath)
{
	while (pc < instructions.size()) {
		instructions[pc]->execute(pc);
	}

	ofstream output(filepath);
	for (int i = 0; i < variables.size(); i++) {
		output << variables[i]->printValue();
		if (i != variables.size() - 1) output << endl;
	}
}

bool Machine::isVariable(string& name)
{
	if (name.size() != 1) return false;
	return (name[0] >= 'A' && name[0] <= 'Z');
}

bool Machine::isInt(string& name)
{
	for (int i = 0; i < name.size(); i++) {
		if (i == 0 && name[i] == '-') continue;
		if (name[i] < '0' || name[i]>'9') return false;
	}
	return true;
}

Variable* Machine::makeVariable(string& name)
{
	if (name == "") return nullptr;
	if (isVariable(name)) return getVariable(name);
	if (isInt(name)) {
		constants.push_back(new IntVariable(stoi(name)));
		constants.back()->setActive(); 
		return constants.back();
	}
	throw SyntaxError("Invalid variable or constant.", instructions.size());
	return nullptr;
}

Variable* Machine::getVariable(string& name){
	int l = 0, r = variables.size();

	while (r > l) {
		int m = (l + r) / 2;
		switch (variables[m]->cmpName(name)) {
		case -1:
			l = m + 1;
			break;
		case 0:
			return variables[m];
			break;
		case 1:
			r = m;
		}
	}

	Variable* new_variable = new IntVariable(name);
	variables.push_back(new_variable);

	for (int i = variables.size() - 2; i >= l; i--) {
		swap(variables[i], variables[i + 1]);
	}
	return variables[l];
}

void Machine::clearMachine()
{
	for (int i = 0; i < variables.size(); i++) {
		delete variables[i];
	}
	for (int i = 0; i < constants.size(); i++) {
		delete constants[i];
	}
	for (int i = 0; i < instructions.size(); i++) {
		delete instructions[i];
	}

	variables.clear();
	constants.clear();
	instructions.clear();
	variable_scope.clear();

	while (instruction_stack.size()) {
		instruction_stack.pop();
	}
	while (active_variables.size()) {
		active_variables.pop();
	}
	pc = 0;
}

void Machine::loadInstruction(string& instruction)
{
	stringstream line(instruction);
	string parameter[MAXINSTRUCTIONSIZE];
	Variable* a, * b, * c;
	a = b = c = nullptr;
	int i = 0, pos;

	while (getline(line, parameter[i], ' ')) {
		i++;
	}
	a = makeVariable(parameter[1]);
	b = makeVariable(parameter[2]);
	c = makeVariable(parameter[3]);

	if (parameter[0] == "SET") {
		loadSet(a, b);
		return;
	}

	if (parameter[0] == "ADD") {
		loadAdd(a, b, c);
		return;
	}

	if (parameter[0] == "SUB") {
		loadSub(a, b, c);
		return;
	}

	if (parameter[0] == "MUL") {
		loadMul(a, b, c);
		return;
	}

	if (parameter[0] == "DIV") {
		loadDiv(a, b, c);
		return;
	}

	if (parameter[0] == "GOTO") {
		loadGoTo(a);
		return;
	}

	if (parameter[0] == "IFGR") {
		loadIfGr(a, b);
		return;
	}

	if (parameter[0] == "IFEQ") {
		loadIfEq(a, b);
		return;
	}

	if (parameter[0] == "ELSE") {
		loadElse();
		return;
	}

	if (parameter[0] == "ENDIF") {
		loadEndIf();
		return;
	}

	if (parameter[0] == "LOOP") {
		loadLoop(a);
		return;
	}

	if (parameter[0] == "ENDLOOP") {
		loadEndLoop();
		return;
	}

	throw SyntaxError("Invalid instruction.", instructions.size() + 1);
}

//Provera da li GoTo preskace inicijalizaciju nekih promenljivih

void Machine::checkGoTo()
{
	for (int i = 0; i < instructions.size(); i++) {
		if (instructions[i]->getType() != GOTO) continue;

		GoTo* ins = dynamic_cast<GoTo*>(instructions[i]);

		for (int j = 0; j < variable_scope.size(); j++) {

			if (i < variable_scope[j].first && i > variable_scope[j].second) {
				int x = i + ins->getMove();

				if (instructions[x]->getType() == ELSE || instructions[x]->getType() == ENDLOOP) {
					throw SyntaxError("Invalid GOTO jump", i + 1);
					return;
				}

				if (x > variable_scope[j].first && x < variable_scope[j].second) {
					throw SyntaxError("Invalid GOTO jump", i + 1);
					return;
				}
			}
		}
	}
}

bool Machine::checkVariable(Variable* a)
{
	if (a == nullptr) return false;
	return a->isActive();
}
