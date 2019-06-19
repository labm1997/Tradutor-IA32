#ifndef HEADER_STATEMENT
#define HEADER_STATEMENT

#include <string>
#include <map>
#include <list>
#include <fstream>
#include "symbol.hpp"

typedef enum {
	NONE,
	SUM
} operation;

typedef enum {
	OK,
	ERROR,
	ERROR_EMPTY
} error;

typedef enum {
	EVAL_UNARY_OK,
	EVAL_UNARY_ERROR_CONVERTION,
	EVAL_UNARY_ERROR_NOT_UNARY,
	EVAL_UNARY_ERROR_EMPTY
} error_evalUnary;

typedef enum {
	EVAL_OK,
	EVAL_ERROR_CONVERTION,
	EVAL_ERROR_EMPTY,
	EVAL_ERROR_SYMBOL_NOT_FOUND
} error_eval;

class Expression {
	public:
	std::string op1;
	std::string op2;
	operation op;
	error_eval eval(SymbolTable, int *);
	error_evalUnary evalUnary(long int *);
};

class Statement {
	public:
	std::string line;
	std::string label;
	std::string op;
	Expression arg[3];
	std::string comment;
	int lineNumber;
	int lineDefinition;
	int countArgs();
	Statement subst(SymbolTable);
	void print();
	static std::list<Statement> getStatementList(std::ifstream *file);
	static std::string renderStatementList(std::list<Statement>);
};

error strToInt(std::string, long int *);

#endif