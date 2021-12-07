# Simple-Compiler
 
Implementation of Machine class which can compile simple code from a file using Machine::loadProgram and execute it using Machine::execute.

This programming language supports the following operations:

| Syntax | Description |
| ----------- | ----------- |
SET A B | A=B
ADD A B C | A=B+C
SUB A B C | A=B-C
MUL A B C | A=B*C
DIV A B C | A=B/C
GOTO A | PC+=A
IFGR A B | If(A>B)
IFEQ A B | If(A=B)
ELSE |
ENFIF |
LOOP | while(true)
LOOP A | for(int i=0;i<A;i++)
ENDLOOP |

Variables A, B and C can either be signed integers or uppercase english letters. If you want variables names to be more than a single letter you can change it inside Machine::isVariable().

| Example input | Example output |
| ----------- | ----------- |
SET A 18 | A=18
SET B 24 | B=24
LOOP | C=6
IFEQ B 0 |
GOTO 9 |
ELSE |
IFGR A B |
SUB A A B |
ELSE |
SUB B B A |
ENDIF |
ENDIF |
ENDLOOP |
SET C A |
SET A 18 |
SET B 24 |
