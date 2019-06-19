#include <iostream>
#include <string>
#include <algorithm>
#include "symbol.hpp"
#include "statement.hpp"
#include "Assembler.hpp"
#include "instruction.hpp"
#include "log.hpp"

static std::map<std::string, Instruction> instructionMap = {
	{"add", Instruction("add", 1, 2, 1, {READ})},
	{"sub", Instruction("sub", 2, 2, 1, {READ})},
	{"mult", Instruction("mult", 3, 2, 1, {READ})},
	{"div", Instruction("div", 4, 2, 1, {READ})},
	{"jmp", Instruction("jmp", 5, 2, 1, {NON})},
	{"jmpn", Instruction("jmpn", 6, 2, 1, {NON})},
	{"jmpp", Instruction("jmpp", 7, 2, 1, {NON})},
	{"jmpz", Instruction("jmpz", 8, 2, 1, {NON})},
	{"copy", Instruction("copy", 9, 3, 2, {READ, WRITE})},
	{"load", Instruction("load", 10, 2, 1, {READ})},
	{"store", Instruction("store", 11, 2, 1, {WRITE})},
	{"input", Instruction("input", 12, 2, 1, {WRITE})},
	{"output", Instruction("output", 13, 2, 1, {READ})},
	{"stop", Instruction("stop", 14, 1, 0, {NON})}
};

void Assembler::addLabel(SymbolTable *ts, Statement it){
	Symbol s;
	s.value = this->address;
	if(ts->symbolExist(it.label)){
		logSemanticError("Using \"" + it.label + "\" from this line from now on", this->lineNumber, it.lineDefinition, it.lineNumber);
	}
	ts->insert(it.label, s);
}

int Assembler::dSECTION(Statement it){
	if(it.countArgs() == 1 && it.arg[0].op == NONE){
		if(it.arg[0].op1 == "text"){
			if(this->text_start != -1){
				logSemanticError("Multiple TEXT sections defined", this->lineNumber, it.lineDefinition, it.lineNumber);
			}
			if(this->data_start != -1){
				this->data_end = this->address-1;
			}
			this->text_start = this->address;
		}
		else if(it.arg[0].op1 == "data"){
			if(this->data_start != -1){
				logSemanticError("Multiple DATA sections defined", this->lineNumber, it.lineDefinition, it.lineNumber);
			}
			if(this->text_start != -1){
				this->text_end = this->address-1;
			}
			this->data_start = this->address;
		}
		else {
			logSyntaxError("invalid argument for section: \"" + it.arg[0].op1 + "\"", this->lineNumber, it.lineDefinition, it.lineNumber);
		}
	}
	if(it.countArgs() != 1) {
		logSyntaxError("wrong number of arguments", this->lineNumber, it.lineDefinition, it.lineNumber);
	}
	if(it.arg[0].op != NONE) {
		logSyntaxError("sum expression not supported for section directive", this->lineNumber, it.lineDefinition, it.lineNumber);
	}
	return 0;
}

int Assembler::dSPACE(Statement it){
	long int nspaces;
	int ret = 0;
	switch(it.arg[0].evalUnary(&nspaces)){
		case EVAL_UNARY_OK:
			for(int i=0 ; i < nspaces ; i++){
				this->dataAddress.push_back(this->address+i);
			}
			ret = nspaces;
		break;
		case EVAL_UNARY_ERROR_EMPTY:
			this->dataAddress.push_back(this->address);
			ret = 1;
		break;
		case EVAL_UNARY_ERROR_CONVERTION:
			logSyntaxError("invalid argument expected integer", this->lineNumber, it.lineDefinition, it.lineNumber);
		break;
		case EVAL_UNARY_ERROR_NOT_UNARY:
			logSyntaxError("argument can't be evaluated on space directive", this->lineNumber, it.lineDefinition, it.lineNumber);
		break;
	}
	return ret;
}

int Assembler::dCONST(){
	this->readAddress.push_back(this->address);
	return 1;
}

SymbolTable Assembler::generateSymbolTable(std::list<Statement> lstmt){
	SymbolTable ts;

	// Initial line and address conditions
	this->address = 0;
	this->lineNumber = 1;

	// Initial text and data positions
	this->text_start = -1;
	this->text_end = -1;
	this->data_start = -1;
	this->data_end = -1;

	for(Statement &it : lstmt){

		// Add label to symbol table
		if(!it.label.empty()) this->addLabel(&ts, it);

		// Calculate address step for instruction
		if(instructionMap.count(it.op) > 0)
			this->address += instructionMap[it.op].size;

		// Calculate address step for section
		else {
			if(it.op == "section")
				this->address += this->dSECTION(it);
			else if(it.op == "space")
				this->address += this->dSPACE(it);
			else if(it.op == "const")
				this->address += this->dCONST();
			else {
				logSemanticError("\"" + it.op + "\" directive or instruction invalid", this->lineNumber, it.lineDefinition, it.lineNumber);
			}
		}

		this->lineNumber++;

	}

	// TEXT end is EOF
	if(this->text_end == -1){
		this->text_end = this->address-1;
	}

	// DATA end is EOF
	if(this->data_end == -1){
		this->data_end = this->address-1;
	}

	// no TEXT segment
	if(this->text_start == -1){
		logSemanticError("Semantic Error: no TEXT section");
	}

	return ts;
}

inline bool Assembler::validWriteAddress(long int arg){
	return std::find(dataAddress.begin(), dataAddress.end(), arg) != dataAddress.end();
}

inline bool Assembler::validReadAddress(long int arg){
	return (std::find(readAddress.begin(), readAddress.end(), arg) != readAddress.end()) || validWriteAddress(arg);
}

inline bool Assembler::validTextAddress(int address){
	return (this->address >= this->text_start && this->address <= this->text_end);
}

inline bool Assembler::validDataAddress(int address){
	return (this->address >= this->data_start && this->address <= this->data_end);
}

std::string Assembler::mINSTRUCTION(SymbolTable ts, Statement it){
	std::string out;
	Instruction in;
	int nargs_stmt;

	if(!this->validTextAddress(this->address)){
		logSemanticError("Invalid section, instruction out of TEXT section", this->lineNumber, it.lineDefinition, it.lineNumber);
	}

	in = instructionMap[it.op];
	nargs_stmt = it.countArgs();
	out += std::to_string(in.opcode) + " ";
	this->address += instructionMap[it.op].size;

	if(in.nargs == nargs_stmt){
		for(int i = 0 ; i < nargs_stmt ; i++){
			int arg;
			switch(it.arg[i].eval(ts, &arg)){
				case EVAL_OK:
					// Avoid zero division
					if(it.op == "div" && arg == 0){
						logSyntaxError("Division by zero", this->lineNumber, it.lineDefinition, it.lineNumber);
					}
					// Avoid write to const address
					if(in.access[i] == WRITE && !validWriteAddress(arg)){
						logSemanticError("Modify readonly (const) address is not allowed", this->lineNumber, it.lineDefinition, it.lineNumber);
					}
					// Avoid read not allocated space
					if(in.access[i] == READ && !validReadAddress(arg)){
						logSemanticError("Access to forbidden address", this->lineNumber, it.lineDefinition, it.lineNumber);
					}
					// Avoid jump out of TEXT
					if(it.op == "jmp" || it.op == "jmpn" || it.op == "jmpp" || it.op == "jmpz"){
						if(!(arg >= text_start && arg <= text_end)){
							logSemanticError("Jump out of TEXT section", this->lineNumber, it.lineDefinition, it.lineNumber);
						}
					}
					out += std::to_string(arg) + " ";
				break;
				case EVAL_ERROR_CONVERTION:
					logSyntaxError("Expected integer, label given as second operand", this->lineNumber, it.lineDefinition, it.lineNumber);
				break;
				case EVAL_ERROR_EMPTY:
					logInternalError(lineNumber, it.lineDefinition, it.lineNumber);
				break;
				case EVAL_ERROR_SYMBOL_NOT_FOUND:
					logSemanticError("Symbol \"" + it.arg[i].op1 + "\" not defined", this->lineNumber, it.lineDefinition, it.lineNumber);
				break;
			}
		}

	}
	else {
		logSyntaxError("\"" + it.op + "\" requires " + std::to_string(in.nargs) + " arguments, " + std::to_string(nargs_stmt) + " given", this->lineNumber, it.lineDefinition, it.lineNumber);
	}

	return out;
}

std::string Assembler::mSPACE(Statement it){
	long int nspaces;
	std::string out;

	if(!this->validDataAddress(this->address)){
		logSemanticError("Invalid section, instruction out of DATA section", this->lineNumber, it.lineDefinition, it.lineNumber);
	}

	switch(it.arg[0].evalUnary(&nspaces)){
		case EVAL_UNARY_OK:
			for(int i=0 ; i < nspaces ; i++) out += "00 ";
			this->address += nspaces;
		break;
		case EVAL_UNARY_ERROR_EMPTY:
			out += "00 ";
			this->address += 1;
		break;
		default:

		break;
	}

	return out;
}

std::string Assembler::mCONST(Statement it){
	std::string out;

	if(!this->validDataAddress(this->address)){
		logSemanticError("Invalid section, instruction out of DATA section", this->lineNumber, it.lineDefinition, it.lineNumber);
	}

	this->address += 1;

	if(it.countArgs() == 1 && it.arg[0].op == NONE){
		long int value;

		switch(it.arg[0].evalUnary(&value)){
			case EVAL_UNARY_OK:
				out += std::to_string(value) + " ";
			break;
			case EVAL_UNARY_ERROR_EMPTY:
				logInternalError(this->lineNumber, it.lineDefinition, it.lineNumber);
			break;
			case EVAL_UNARY_ERROR_CONVERTION:
				logSyntaxError("invalid argument expected integer", this->lineNumber, it.lineDefinition, it.lineNumber);
			break;
			case EVAL_UNARY_ERROR_NOT_UNARY:
				logSyntaxError("argument can't be evaluated on const directive", this->lineNumber, it.lineDefinition, it.lineNumber);
			break;
		}
	}
	if(it.countArgs() != 1) {
		logSyntaxError("wrong number of arguments", this->lineNumber, it.lineDefinition, it.lineNumber);
	}
	if(it.arg[0].op != NONE) {
		logSyntaxError("sum expression not supported for const directive", this->lineNumber, it.lineDefinition, it.lineNumber);
	}

	return out;
}

std::string Assembler::generateObjectCode(SymbolTable ts, std::list<Statement> lstmt){
	std::string out;

	this->address = 0;
	this->lineNumber = 1;

	for(Statement &it : lstmt){

		// Instruction rendering
		if(instructionMap.count(it.op) > 0)
			out += this->mINSTRUCTION(ts, it);
		// Directive
		else {
			if(it.op == "section");
			else if(it.op == "space")
				out += this->mSPACE(it);
			else if(it.op == "const")
				out += this->mCONST(it);
		}
		
		this->lineNumber++;
		
	}

	return out;
}