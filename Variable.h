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

	//Ukoliko se za neki tip variable ove operacije pozivaju, a nisu redefinisane y ogovarajucoj klasi potrebno je signalizirati gresku
	virtual void add(Variable* a, Variable* b);
	virtual void sub(Variable* a, Variable* b);
	virtual void mul(Variable* a, Variable* b);
	virtual void div(Variable* a, Variable* b);

	virtual void set(Variable* val);
	virtual int cmp(Variable* val);

	virtual string printValue();
	int cmpName(string& s);

	int castToInt();
	
protected:
	string name;
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