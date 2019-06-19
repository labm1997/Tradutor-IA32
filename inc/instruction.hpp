#ifndef HEADER_INSTRUCTION
#define HEADER_INSTRUCTION

#include <string>
#include <array>

typedef enum {
	READ,
	WRITE,
	NON
} dataAccess;

class Instruction {
	public:
	std::string mne;
	int size;
	int opcode;
	int nargs;
	dataAccess access[3];
	Instruction(std::string, int, int, int, std::initializer_list<dataAccess>);
	Instruction() { };
};


#endif