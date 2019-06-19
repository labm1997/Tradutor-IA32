#ifndef HEADER_PP
#define HEADER_PP

#include <string>
#include <list>
#include <map>
#include "statement.hpp"


typedef enum {
	NORMAL, 
	MACRO,
	IFFALSE
} ppState;

class MDT {
	public:
	int nargs;
	std::string arg[3];
	std::list<Statement> lstmt;
};

class PreProcessor {
	private:
	SymbolTable ppts;
	std::string macrolabel;
	ppState state = NORMAL;
	MDT mdt;
	std::list<Statement> macroList;
	std::map<std::string, MDT> ppMNT;
	std::list<Statement> pptoRender;
	public:
	void renderStatements(std::list<Statement>);
	void dEQU(Statement);
	void dMACRO(Statement);
	void dMACROEXPAND(Statement);
	void dENDMACRO(Statement);
	void dAPPENDMACRO(Statement);
	void dIF(Statement);
	void dAPPEND(Statement);
	std::list<Statement> getResult();
};

#endif
