#include "Command.h"
#include "Exceptions.h"

CommandType Command::getType()
{
	return type;
}

Set::Set(Variable* var_a, Variable* var_b)
{
	a = var_a;
	b = var_b;
	type = SET;
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
	type = ADD;
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
	type = SUB;
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
	type = MUL;
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
	type = DIV;
}

void Div::execute(int& pc)
{
	if (c->castToInt() == 0) {
		throw ExecutionError("Division by zero.", pc + 1);
		return;
	}

	a->div(b, c);
	pc++;
}

GoTo::GoTo(int pos)
{
	move = pos;
	type = GOTO;
}

void GoTo::execute(int& pc)
{
	pc += move;
}

int GoTo::getMove()
{
	return move;
}

IfGr::IfGr(Variable* var_a, Variable* var_b)
{
	a = var_a;
	b = var_b;
	type = IFGR;
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
	type = IFEQ;
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
	type = ELSE;
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
	//0 oznacava beskonacnu petlju
	num = 0;
	type = LOOP;
}

Loop::Loop(int val)
{
	num = val;
	type = LOOP;
}

void Loop::setEndLoop(int pos, EndLoop* end_loop)
{
	pos_endloop = pos;
	endloop = end_loop;
}

void Loop::execute(int& pc)
{
	endloop->setCnt(num);
	pc++;
}

EndLoop::EndLoop(int loop)
{
	pos_loop = loop;
	cnt = -1;
	type = ENDLOOP;
}

void EndLoop::setCnt(int val)
{
	cnt = val;
}

void EndLoop::execute(int& pc)
{
	if (cnt == 0) {
		pc = pos_loop + 1;
		return;
	}

	//1 oznacava poslednje izvrsavanje te petlje
	if (cnt == 1) pc++;
	else {
		pc = pos_loop + 1;
		cnt--;
	}
}
