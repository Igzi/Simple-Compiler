#include "Command.h"

Set::Set(Variable* var_a, Variable* var_b)
{
	a = var_a;
	b = var_b;
}

void Set::execute(int& pc)
{
	a->set(b);
	pc++;
}

Add::Add(Variable* var_a, Variable* var_b, Variable* var_c)
{
	a = var_a;
	b = var_b;
	c = var_c;
}

void Add::execute(int& pc)
{
	a->add(b, c);
	pc++;
}

Sub::Sub(Variable* var_a, Variable* var_b, Variable* var_c)
{
	a = var_a;
	b = var_b;
	c = var_c;
}

void Sub::execute(int& pc)
{
	a->sub(b, c);
	pc++;
}

Mul::Mul(Variable* var_a, Variable* var_b, Variable* var_c)
{
	a = var_a;
	b = var_b;
	c = var_c;
}

void Mul::execute(int& pc)
{
	a->mul(b, c);
	pc++;
}

Div::Div(Variable* var_a, Variable* var_b, Variable* var_c)
{
	a = var_a;
	b = var_b;
	c = var_c;
}

void Div::execute(int& pc)
{
	a->div(b, c);
	pc++;
}

GoTo::GoTo(int pos)
{
	move = pos;
}

void GoTo::execute(int& pc)
{
	pc += move;
}

IfGr::IfGr(Variable* var_a, Variable* var_b)
{
	a = var_a;
	b = var_b;
}

void IfGr::setPosElse(int pos)
{
	pos_else = pos;
}

void IfGr::execute(int& pc)
{
	if (a->cmp(b)!=1) pc = pos_else;
	pc++;
}

IfEq::IfEq(Variable* var_a, Variable* var_b)
{
	a = var_a;
	b = var_b;
}

void IfEq::setPosElse(int pos)
{
	pos_else = pos;
}

void IfEq::execute(int& pc)
{
	if (a->cmp(b) != 0) pc = pos_else;
	pc++;
}

void Else::setPosEndIf(int pos)
{
	pos_endif = pos;
}

void Else::execute(int& pc)
{
	pc = pos_endif;
}

void EndIf::execute(int& pc)
{
	pc++;
}

Loop::Loop()
{
	//-1 oznacava beskonacnu petlju
	num = -1;
}

Loop::Loop(int val)
{
	cnt = 0;
	num = val;
}

void Loop::setPosEndLoop(int pos)
{
	pos_endloop = pos;
}

void Loop::execute(int& pc)
{
	if (num != -1 && cnt == num) {
		pc = pos_endloop;
		cnt = 0;
	}
	pc++;
}

EndLoop::EndLoop(int loop)
{
	pos_loop = loop;
}

void EndLoop::execute(int& pc)
{
	pc = pos_loop;
}
