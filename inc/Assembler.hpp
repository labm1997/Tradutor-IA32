#ifndef HEADER_ASSEMBLER
#define HEADER_ASSEMBLER

#include <list>
#include <string>
#include "symbol.hpp"
#include "statement.hpp"

typedef enum {
	SECTION_NONE,
	SECTION_TEXT,
	SECTION_DATA
} section;

class Assembler {
	private:
	int text_start, text_end;
	int data_start, data_end;
	bool textHappened = false;
	bool dataHappened = false;
	int lineNumber;
	int address;
	std::list<long int> dataAddress;
	std::list<long int> readAddress;
	void addLabel(SymbolTable *, Statement);
	int dSECTION(Statement);
	int dSPACE(Statement);
	int dCONST();
	bool validWriteAddress(long int);
	inline bool validTextAddress(int);
	inline bool validDataAddress(int);
	std::string mINSTRUCTION(SymbolTable, Statement);
	std::string mSPACE(Statement);
	std::string mCONST(Statement);

	public:
	SymbolTable generateSymbolTable(std::list<Statement>);
	std::string generateObjectCode(SymbolTable, std::list<Statement>);
	bool validDataAddress(long int);
	bool validReadAddress(long int);

};

#endif