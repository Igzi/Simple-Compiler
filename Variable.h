#ifndef variable_h
#define variable_h

//Ovaj fajl podrzava dodavanje novih tipova promenljivih bez vecih izmena u ostalim delovima koda
//i obradjuje greske koje mogu nastati prilikom izvrsavanja koda

#include <string>

using namespace std;

enum VariableType {
	INT,
	NOTDEFINED,
};

class Variable {
public:
	Variable();

	virtual void add(Variable* a, Variable* b);
	virtual void sub(Variable* a, Variable* b);
	virtual void mul(Variable* a, Variable* b);
	virtual void div(Variable* a, Variable* b);

	virtual void set(Variable* val);
	virtual int cmp(Variable* val);

	virtual string printValue();

	void setActive();
	void setInactive();
	bool isActive();
	bool isConstant();

	int cmpName(string& s);
	int castToInt();
	
protected:
	string name;
	bool active, constant;
	VariableType type;
};

class IntVariable : public Variable {
	friend class Variable;
public:
	IntVariable(string& var_name);
	IntVariable(int val);

	virtual void add(Variable* a, Variable* b) override;
	virtual void sub(Variable* a, Variable* b) override;
	virtual void mul(Variable* a, Variable* b) override;
	virtual void div(Variable* a, Variable* b) override;

	virtual void set(Variable* val) override;
	virtual int cmp(Variable* val) override;

	virtual string printValue() override;
private:
	int value;
};

#endif // variable_h