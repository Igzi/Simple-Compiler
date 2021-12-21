#include "Machine.h"
#include "Exceptions.h"

//Fajl koji sadrzi metode za ucitavanje instrukcija

void Machine::loadSet(Variable* a, Variable* b)
{
	if (a == nullptr || a->isConstant() || !checkVariable(b)) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
		return;
	}

	if (!a->isActive()) {
		a->setActive();
		active_variables.push({ instructions.size(), a });
	}

	instructions.push_back(new Set(a, b));
}

void Machine::loadAdd(Variable* a, Variable* b, Variable* c)
{
	if (!checkVariable(a) || !checkVariable(b) || !checkVariable(c) || a->isConstant()) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instructions.push_back(new Add(a, b, c));
}

void Machine::loadSub(Variable* a, Variable* b, Variable* c)
{
	if (!checkVariable(a) || !checkVariable(b) || !checkVariable(c) || a->isConstant()) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instructions.push_back(new Sub(a, b, c));
}

void Machine::loadMul(Variable* a, Variable* b, Variable* c)
{
	if (!checkVariable(a) || !checkVariable(b) || !checkVariable(c) || a->isConstant()) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instructions.push_back(new Mul(a, b, c));
}

void Machine::loadDiv(Variable* a, Variable* b, Variable* c)
{
	if (!checkVariable(a) || !checkVariable(b) || !checkVariable(c) || a->isConstant()) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instructions.push_back(new Div(a, b, c));
}

void Machine::loadGoTo(Variable* a)
{
	if (a == nullptr || !a->isConstant()) {
		throw SyntaxError("Invalid instruction operand.", instructions.size() + 1);
		return;
	}

	if(a->castToInt() == 0) {
		throw SyntaxError("GOTO value can not be 0.", instructions.size() + 1);
		return;
	}

	instructions.push_back(new GoTo(a->castToInt()));
}

void Machine::loadIfGr(Variable* a, Variable* b)
{
	if (!checkVariable(a) || !checkVariable(b)) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instruction_stack.push({ instructions.size(),"IFGR" });
	instructions.push_back(new IfGr(a, b));
}

void Machine::loadIfEq(Variable* a, Variable* b)
{
	if (!checkVariable(a) || !checkVariable(b)) {
		throw SyntaxError("Invalid or undeclared instruction operand.", instructions.size() + 1);
	}

	instruction_stack.push({ instructions.size(),"IFEQ" });
	instructions.push_back(new IfEq(a, b));
}

void Machine::loadElse()
{
	if (instruction_stack.empty()) {
		throw SyntaxError("Unmatched ELSE instruction.", instructions.size() + 1);
		return;
	}

	pair<int, string> tmp = instruction_stack.top();
	instruction_stack.pop();
	int pos = tmp.first;
	string name = tmp.second;

	if (name != "IFGR" && name != "IFEQ") {
		throw SyntaxError("Unmatched ELSE instruction.", instructions.size() + 1);
		return;
	}

	if (name == "IFGR") {
		IfGr* ifgr = dynamic_cast<IfGr*>(instructions[pos]);
		ifgr->setPosElse(instructions.size());
	}
	if (name == "IFEQ") {
		IfEq* ifeq = dynamic_cast<IfEq*>(instructions[pos]);
		ifeq->setPosElse(instructions.size());
	}

	while (active_variables.size() && active_variables.top().first > pos) {
		variable_scope.push_back({ active_variables.top().first, instructions.size() + 1});
		active_variables.top().second->setInactive();
		active_variables.pop();
	}

	instruction_stack.push({ instructions.size(),"ELSE" });
	instructions.push_back(new Else());
}

void Machine::loadEndIf()
{
	if (instruction_stack.empty()) {
		throw SyntaxError("Unmatched ENDIF instruction.", instructions.size() + 1);
		return;
	}

	pair<int, string> tmp = instruction_stack.top();
	instruction_stack.pop();
	int pos = tmp.first;
	string name = tmp.second;

	if (name != "IFGR" && name != "IFEQ" && name != "ELSE") {
		throw SyntaxError("Unmatched ELSE instruction.", instructions.size() + 1);
		return;
	}

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

	while (active_variables.size() && active_variables.top().first > pos) {
		variable_scope.push_back({ active_variables.top().first, instructions.size()});
		active_variables.top().second->setInactive();
		active_variables.pop();
	}

	instructions.push_back(new EndIf());
}

void Machine::loadLoop(Variable* a)
{
	if (a != nullptr && !a->isConstant()) {
		throw SyntaxError("Invalid instruction operand.", instructions.size() + 1);
		return;
	}

	if (a != nullptr && a->castToInt() <= 0) {
		throw SyntaxError("LOOP value must be positive.", instructions.size() + 1);
		return;
	}

	instruction_stack.push({ instructions.size(),"LOOP" });
	//if(a!=nullptr) active_variables.push({ instructions.size(),a }); Ova linija odredjuje da li je dozvoljeno uskakanje u petlje koje nisu beskonacne.

	if (a == nullptr) instructions.push_back(new Loop());
	else instructions.push_back(new Loop(a->castToInt()));
}

void Machine::loadEndLoop()
{
	if (instruction_stack.empty()) {
		throw SyntaxError("Unmatched ENDLOOP instruction.", instructions.size() + 1);
		return;
	}

	pair<int, string> tmp = instruction_stack.top();
	instruction_stack.pop();

	int pos = tmp.first;
	string name = tmp.second;
	EndLoop* endloop = new EndLoop(pos);

	if (name == "LOOP") {
		Loop* loop = dynamic_cast<Loop*>(instructions[pos]);
		loop->setEndLoop(instructions.size(), endloop);
	}
	else {
		throw SyntaxError("Unmatched ENDLOOP instruction.", instructions.size() + 1);
		return;
	}

	while (active_variables.size() && active_variables.top().first > pos) {
		variable_scope.push_back({ active_variables.top().first, instructions.size() + 1});
		active_variables.top().second->setInactive();
		active_variables.pop();
	}

	instructions.push_back(endloop);
}