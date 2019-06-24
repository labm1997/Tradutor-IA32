#include <iostream>
#include <fstream>
#include <regex>
#include <list>
#include <algorithm>
#include <string>
#include <map>
#include "statement.hpp"
#include "symbol.hpp"
#include "pp.hpp"
#include "Assembler.hpp"

using namespace std;

int main(int argc, char** argv) {
	ifstream file;
	list<Statement> statements;
	list<Statement> ppStatements;
	PreProcessor pp;
	Assembler as;
	SymbolTable as_symbols;
	
	
	if(argc < 2) {
		cout << "Usage: ./" << argv[0] << " FILE\n";
		return 1;
	}
	file.open(argv[1]);
	if(!file.is_open()) {
		cout << "Failed to open \"" << argv[1] << "\"\n";
		return 1;
	}
	statements = Statement::getStatementList(&file);
	// for(Statement &it: statements){
	// 	it.print();
	// }
	file.close();
	
	pp.renderStatements(statements);
	ppStatements = pp.getResult();
	
	// for(Statement &it: ppStatements){
	// 	it.print();
	// }
	
	string fileName = argv[1];
	size_t position = fileName.find(".");
	string extractName = (string::npos == position)? fileName : fileName.substr(0, position);

	ofstream ppfile;
	ppfile.open(extractName + ".pre");
	ppfile << Statement::renderStatementList(ppStatements);
	ppfile.close();
	
	
	as_symbols = as.generateSymbolTable(ppStatements);

	//as_symbols.print();
	ofstream ofile;
	ofile.open(extractName + ".obj");
	ofile << as.translateIA32(as_symbols, ppStatements);
	ofile.close();


	return 0;
}
