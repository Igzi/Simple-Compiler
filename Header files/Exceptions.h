#ifndef exceptions_h
#define exceptions_h

#include <exception>

using namespace std;

class SyntaxError : public exception {
public:
	SyntaxError(const char* msg, int pos) : exception(msg) {
		line = pos;
	};

	int getLine() {
		return line;
	};

private:
	int line;
};

class ExecutionError : public exception {
public:
	ExecutionError(const char* msg, int pos) : exception(msg) {
		line = pos;
	};

	int getLine() {
		return line;
	};

private:
	int line;
};

#endif // exceptions_h
