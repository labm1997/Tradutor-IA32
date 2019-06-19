#include "instruction.hpp"

Instruction::Instruction(std::string _mne, int _opcode, int _size, int _nargs, std::initializer_list<dataAccess> _access){
	mne = _mne;
	opcode = _opcode;
	size = _size;
	nargs = _nargs;
	// int i=0;
	// for(const dataAccess *it = _access.begin(); it != _access.end() ; ++it, i++){
	// 	access[i] = *it;
	// }
	std::copy(_access.begin(), _access.end(), access);
}