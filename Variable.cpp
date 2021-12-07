#include "Variable.h"

Variable::Variable()
{
	name = "";
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
	value = 0;
}

IntVariable::IntVariable(int val)
{
	type = INT;
	name = "const";
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
