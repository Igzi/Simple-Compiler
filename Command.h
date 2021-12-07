#ifndef command_h
#define command_h

#include "Variable.h"

class Command {
public:
	virtual void execute(int& pc) = 0;
};

class Set : public Command {
public:
	Set(Variable* var_a, Variable* var_b);
	virtual void execute(int& pc) override;

private:
	Variable* a, * b;
};

class Add : public Command {
public:
	Add (Variable* var_a, Variable* var_b,Variable* var_c);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b, * c;
};

class Sub : public Command {
public:
	Sub(Variable* var_a, Variable* var_b, Variable* var_c);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b, * c;
};

class Mul : public Command {
public:
	Mul(Variable* var_a, Variable* var_b, Variable* var_c);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b, * c;
};

class Div : public Command {
public:
	Div(Variable* var_a, Variable* var_b, Variable* var_c);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b, * c;
};

class GoTo : public Command {
public:
	GoTo(int pos);

	virtual void execute(int& pc) override;

protected:
	int move;
};

class IfGr : public Command {
public:
	IfGr(Variable* var_a, Variable* var_b);

	void setPosElse(int pos);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b;
	int pos_else;
};

class IfEq : public Command {
public:
	IfEq(Variable* var_a, Variable* var_b);

	void setPosElse(int pos);

	virtual void execute(int& pc) override;

protected:
	Variable* a, * b;
	int pos_else;
};

class Else : public Command {
public:
	void setPosEndIf(int pos);

	virtual void execute(int& pc) override;

protected:
	int pos_endif;
};

class EndIf : public Command {
public:
	virtual void execute(int& pc) override;
};

class Loop : public Command {
public:
	Loop();
	Loop(int val);

	void setPosEndLoop(int pos);

	virtual void execute(int& pc) override;

protected:
	int cnt, num, pos_endloop;
};

class EndLoop : public Command {
public:
	EndLoop(int loop);

	virtual void execute(int& pc) override;

protected:
	int pos_loop;
};

#endif // command_h