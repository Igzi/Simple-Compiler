#include "Machine.h"

#include <sstream>
#include <fstream>

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
		loadInstruction(instruction);
	}
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
		return constants.back();
	}
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
	pc = 0;
}

void Machine::loadInstruction(string& instruction)
{
	stringstream line(instruction);
	string parameter[MAXINSTRUCTIONSIZE], name;
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
		instructions.push_back(new Set(a, b));
	}
	if (parameter[0] == "ADD") {
		instructions.push_back(new Add(a, b, c));
	}
	if (parameter[0] == "SUB") {
		instructions.push_back(new Sub(a, b, c));
	}
	if (parameter[0] == "MUL") {
		instructions.push_back(new Mul(a, b, c));
	}
	if (parameter[0] == "DIV") {
		instructions.push_back(new Div(a, b, c));
	}
	if (parameter[0] == "GOTO") {
		instructions.push_back(new GoTo(a->castToInt()));
	}
	if (parameter[0] == "IFGR") {
		instruction_stack.push({ instructions.size(),"IFGR" });
		instructions.push_back(new IfGr(a, b));
	}
	if (parameter[0] == "IFEQ") {
		instruction_stack.push({ instructions.size(),"IFEQ" });
		instructions.push_back(new IfEq(a, b));
	}
	if (parameter[0] == "ELSE") {
		pair<int,string> tmp = instruction_stack.top();
		instruction_stack.pop();
		pos = tmp.first;
		name = tmp.second;

		if (name == "IFGR") {
			IfGr* ifgr = dynamic_cast<IfGr*>(instructions[pos]);
			ifgr->setPosElse(instructions.size());
		}
		if (name == "IFEQ") {
			IfEq* ifeq = dynamic_cast<IfEq*>(instructions[pos]);
			ifeq->setPosElse(instructions.size());
		}
		instruction_stack.push({ instructions.size(),"ELSE" });
		instructions.push_back(new Else());
	}
	if (parameter[0] == "ENDIF") {
		pair<int, string> tmp = instruction_stack.top();
		instruction_stack.pop();
		pos = tmp.first;
		name = tmp.second;

		if (name == "IFGR") {
			IfGr* ifgr = dynamic_cast<IfGr*>(instructions[pos]);
			ifgr->setPosElse(instructions.size());
		}
		if (name == "IFEQ") {
			IfEq* ifeq = dynamic_cast<IfEq*>(instructions[pos]);
			ifeq->setPosElse(instructions.size());
		}
		if (name == "ELSE") {
			Else* ifelse = dynamic_cast<Else*>(instructions[pos]);
			ifelse->setPosEndIf(instructions.size());
		}
		instructions.push_back(new EndIf());
	}
	if (parameter[0] == "LOOP") {
		instruction_stack.push({ instructions.size(),"LOOP" });
		if (a == nullptr) instructions.push_back(new Loop());
		else instructions.push_back(new Loop(a->castToInt()));
	}
	if (parameter[0] == "ENDLOOP") {
		pair<int, string> tmp = instruction_stack.top();
		instruction_stack.pop();
		pos = tmp.first;
		name = tmp.second;

		if (name == "LOOP") {
			Loop* loop = dynamic_cast<Loop*>(instructions[pos]);
			loop->setPosEndLoop(instructions.size());
		}
		instructions.push_back(new EndLoop(pos));
	}
}
