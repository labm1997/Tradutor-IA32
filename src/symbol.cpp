#include <iostream>
#include "symbol.hpp"
#include "statement.hpp"

bool Symbol::checkLabel(std::string label){

	if(label[0] >= '0' && label[0] <= '9'){
		return false;
	}

	return strToInt(label, NULL) != OK;
}

void SymbolTable::insert(std::string label, Symbol s){
	this->ts.insert(std::pair<std::string,Symbol>(label, s));
}

bool SymbolTable::symbolExist(std::string label){
	return this->ts.count(label) > 0;
}

Symbol SymbolTable::getSymbol(std::string label){
	return this->ts[label];
}

void SymbolTable::print(){
	for(auto it = this->ts.begin() ; it != this->ts.end() ; it++){
		std::cout << it->first << ": " << it->second.value << "\n";
	}
}