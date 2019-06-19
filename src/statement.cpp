#include "statement.hpp"
#include "symbol.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include "log.hpp"

// !TODO: Don't put numbers on TS!!!!

error_eval Expression::eval(SymbolTable ts, int *ret){
	long int v_op1;
	long int v_op2 = 0;
	error e = strToInt(op1, &v_op1);

	if(e == ERROR_EMPTY) return EVAL_ERROR_EMPTY;

	if(e == ERROR){
		if(ts.symbolExist(op1)){
			v_op1 = ts.getSymbol(op1).value;
		}
		else {
			return EVAL_ERROR_SYMBOL_NOT_FOUND;
		}
	}

	if(op != NONE){
		e = strToInt(op2, &v_op2);
		if(e != OK) return EVAL_ERROR_CONVERTION;
	}

	*ret = v_op1 + v_op2;
	return EVAL_OK;
}

error_evalUnary Expression::evalUnary(long int *ret){
	if(op != NONE){
		return EVAL_UNARY_ERROR_NOT_UNARY;
	}
	if(op1.empty()) return EVAL_UNARY_ERROR_EMPTY;
	if(strToInt(op1, ret) != OK) return EVAL_UNARY_ERROR_CONVERTION;
	return EVAL_UNARY_OK;
}

std::string strToLower(std::string data){
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

error strToInt(std::string str, long int *ret){
	char *c = (char *)str.c_str();
	char *a = c;
	bool hexa = false;
	
	if(*c == '\0') return ERROR_EMPTY;

	if(*c == '-') c++;
	// Hexa
	if(*c == '0' && *(c+1) == 'x') {
		hexa = true;
		c = c+2;
	}
	
	while(*c != '\0'){
		if(!((*c >= '0' && *c <= '9') || (hexa && (*c >= 'a' && *c <= 'f')) ))
			return ERROR;
		c++;
	}
	
	if(ret) {
		*ret = hexa ? strtol(a, NULL, 16) : strtol(a, NULL, 10);
	}
	
	return OK;
}

int Statement::countArgs(){
	if(this->arg[0].op1.empty()) return 0;
	else if(this->arg[1].op1.empty()) return 1;
	else if(this->arg[2].op1.empty()) return 2;
	else return 3;
}

Statement Statement::subst(SymbolTable toSubst){
	Statement ret = *this;
	for(auto it = toSubst.ts.begin() ; it != toSubst.ts.end() ; it++){
		for(int i=0 ; i<3 ; i++){
			if(ret.arg[i].op1 == it->first)
				ret.arg[i].op1 = it->second.str;
			if(ret.arg[i].op2 == it->first)
				ret.arg[i].op2 = it->second.str;
		}
	}
	return ret;
}

void Statement::print(){
	std::cout << "Line: \"" << (*this).line << "\"\n";
	std::cout << "\tLine Number: " << (*this).lineNumber << "\n";
	std::cout << "\tLabel: \"" << (*this).label << "\"\n";
	std::cout << "\tOperation: \"" << (*this).op << "\"\n";
	std::cout << "\targ[0]_op1: \"" << (*this).arg[0].op1 << "\"\n";
	std::cout << "\targ[0]_op: " << ((*this).arg[0].op ? "SUM" : "NONE") << "\n";
	std::cout << "\targ[0]_op2: \"" << (*this).arg[0].op2 << "\"\n";
	std::cout << "\targ[1]_op1: \"" << (*this).arg[1].op1 << "\"\n";
	std::cout << "\targ[1]_op: " << ((*this).arg[1].op ? "SUM" : "NONE") << "\n";
	std::cout << "\targ[1]_op2: \"" << (*this).arg[1].op2 << "\"\n";
	std::cout << "\targ[2]_op1: \"" << (*this).arg[2].op1 << "\"\n";
	std::cout << "\targ[2]_op: " << ((*this).arg[2].op ? "SUM" : "NONE") << "\n";
	std::cout << "\targ[2]_op2: \"" << (*this).arg[2].op2 << "\"\n";
	std::cout << "\tComment: \"" << (*this).comment << "\"\n";
}

std::list<Statement> Statement::getStatementList(std::ifstream *file){
	std::string rejectSpaces = "(?:\\s|\\t)*";
	std::string matchLabel = rejectSpaces + "(?:(\\w*):)?" + rejectSpaces + "(?:(\\w*):)?" + rejectSpaces + "(?:(?:\\w*):)*" + rejectSpaces;
	std::string matchOperation = "([\\w|&]*)";
	std::string matchFirstOp = rejectSpaces + "(?:(?:\\+)?((?:-)?[\\w|&]*))" + rejectSpaces;
	std::string matchArg1 = "(?:"  + matchFirstOp + "(?:(\\+)" + rejectSpaces + "([\\w|&]*))?)?" + rejectSpaces;
	std::string matchArg2 = "(?:," + matchFirstOp + "(?:(\\+)" + rejectSpaces + "([\\w|&]*))?)?" + rejectSpaces;
	std::string matchArg3 = "(?:," + matchFirstOp + "(?:(\\+)" + rejectSpaces + "([\\w|&]*))?)?" + rejectSpaces;
	std::string matchRest = "([^;]*)(?:;(.*))?";
	std::regex matchStatement(matchLabel + matchOperation + matchArg1 + matchArg2 + matchArg3 + matchRest);
	
	std::list<Statement> statements;
	std::smatch lineMatch;
	std::string line;

	for(int lineNumber=1 ; std::getline(*file, line) ; lineNumber++){
	
		if(std::regex_search(line, lineMatch, matchStatement)){
			Statement statement;
			Expression arg[3];
			statement.line = line;
			statement.label = strToLower(lineMatch.str(1));
			if(!statement.label.empty() && !Symbol::checkLabel(statement.label))
				logparserSyntaxError("Invalid label \"" + statement.label + "\"", lineNumber);
				//std::cout << "Syntax Error: Invalid label in line " << lineNumber << "\n";
			if(!lineMatch.str(2).empty()) 
				logparserSyntaxError("Multiple labels in the same statement", lineNumber);
				//std::cout << "Syntax Error: Multiple labels in the same statement in line " << lineNumber << "\n";
			statement.op = strToLower(lineMatch.str(3));
			arg[0].op1 = strToLower(lineMatch.str(4));
			if(!lineMatch.str(5).empty()){
				if(lineMatch.str(6).empty()){
					logparserSyntaxError("Missing second argument after + operator", lineNumber);
					//std::cout << "Syntax Error: Missing second argument after + operator, in line " << lineNumber << "\n";
				}
				else {
					arg[0].op = SUM;
					arg[0].op2 = strToLower(lineMatch.str(6));
				}
			}
			else arg[0].op = NONE;
			arg[1].op1 = strToLower(lineMatch.str(7));
			if(!lineMatch.str(8).empty()){
				if(lineMatch.str(9).empty()){
					logparserSyntaxError("Missing second argument after + operator", lineNumber);
					//std::cout << "Syntax Error: Missing second argument after + operator, in line " << lineNumber << "\n";
				}
				else {
					arg[1].op = SUM;
					arg[1].op2 = strToLower(lineMatch.str(9));
				}
			}
			else arg[1].op = NONE;
			arg[2].op1 = strToLower(lineMatch.str(10));
			if(!lineMatch.str(11).empty()){
				if(lineMatch.str(12).empty()){
					logparserSyntaxError("Missing second argument after + operator", lineNumber);
					//std::cout << "Syntax Error: Missing second argument after + operator, in line " << lineNumber << "\n";
				}
				else {
					arg[2].op = SUM;
					arg[2].op2 = strToLower(lineMatch.str(12));
				}
			}
			else arg[2].op = NONE;
			statement.arg[0] = arg[0];
			statement.arg[1] = arg[1];
			statement.arg[2] = arg[2];

			if(!lineMatch.str(13).empty()){
				logparserSyntaxError("Unexpected content \"" + lineMatch.str(13) + "\"", lineNumber);
				//std::cout << "Syntax Error: Unexpected content \"" << lineMatch.str(13) << "\", in line " << lineNumber << "\n";
			}

			statement.comment = lineMatch.str(14);
			statement.lineNumber = lineNumber;
			statement.lineDefinition = lineNumber;
			//statement.print();
			statements.push_back(statement);
		}
		else {
			std::cout << "\tINVALID SYNTAX\n";
		}
	}
	return statements;
}


std::string Statement::renderStatementList(std::list<Statement> lstmt){
	std::string out;
	for(Statement &it : lstmt){
		if(!it.label.empty()){
			out +=  it.label + ": ";
		}
		if(!it.op.empty()){
			out += it.op;
		}
		for(int i=0 ; i<3 ; i++){
			if(!it.arg[i].op1.empty()){
				if(i != 0) out += ",";
				out += " " + it.arg[i].op1;
			}
			if(it.arg[i].op == SUM && !it.arg[i].op2.empty()){
				out += "+" + it.arg[i].op2;
			}
		}
		out += "\n";
	}
	return out;
}
