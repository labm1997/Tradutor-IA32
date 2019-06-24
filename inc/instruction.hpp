#ifndef HEADER_INSTRUCTION
#define HEADER_INSTRUCTION

#include <string>
#include <array>
#include <functional>
#include "statement.hpp"

typedef enum {
	READ,
	WRITE,
	NON
} dataAccess;

typedef std::function<std::string(Statement)> InstructionCallback;

class Instruction {
	public:
	std::string mne;
	int size;
	int opcode;
	int nargs;
	dataAccess access[3];
	Instruction(std::string, int, int, int, std::initializer_list<dataAccess>);
	Instruction(std::string, int, int, int, std::initializer_list<dataAccess>, InstructionCallback);
	InstructionCallback callback;
	Instruction() { };
};


#endif