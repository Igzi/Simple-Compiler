#include "Variable.h"

Variable::Variable()
{
	name = "";
	active = false;
	constant = false;
	type = NOTDEFINED;
}

void Variable::add(Variable* a, Variable* b)
{
}

void Variable::sub(Variable* a, Variable* b)
{
}

void Variable::mul(Variable* a, Variable* b)
{
}

void Variable::div(Variable* a, Variable* b)
{
}

void Variable::set(Variable* val)
{
}

int Variable::cmp(Variable* val)
{
	return 0;
}

string Variable::printValue()
{
	return "";
}

void Variable::setActive()
{
	if(!constant) active = true;
}

void Variable::setInactive()
{
	if(!constant) active = false;
}

bool Variable::isActive()
{
	return active;
}

bool Variable::isConstant()
{
	return constant;
}

int Variable::cmpName(string& s)
{
	if (name < s) return -1;
	if (name == s) return 0;
	return 1;
}

int Variable::castToInt()
{
	IntVariable* var = dynamic_cast<IntVariable*>(this);
	return var->value;
}

IntVariable::IntVariable(string& var_name)
{
	type = INT;
	name = var_name;
	active = constant = false; //Comstruktor bez vrednosti se moze pozvati samo za promenljive
	value = 0;
}

IntVariable::IntVariable(int val)
{
	type = INT;
	name = "const";
	active = constant = true; //Comstruktor bez imena se moze pozvati samo za konstante
	value = val;
}

void IntVariable::add(Variable* a, Variable* b)
{
	value = a->castToInt() + b->castToInt();
}

void IntVariable::sub(Variable* a, Variable* b)
{
	value = a->castToInt() - b->castToInt();
}

void IntVariable::mul(Variable* a, Variable* b)
{
	value = a->castToInt() * b->castToInt();
}

void IntVariable::div(Variable* a, Variable* b)
{
	value = a->castToInt() / b->castToInt();
}

void IntVariable::set(Variable* val)
{
	value = val->castToInt();
}

int IntVariable::cmp(Variable* val)
{
	int int_val = val->castToInt();
	if (value < int_val) return -1;
	if (int_val == value) return 0;
	return 1;
}

string IntVariable::printValue()
{
	return name + "=" + to_string(value);
}
