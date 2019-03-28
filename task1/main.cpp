#include "Assembler.h"
#include "Disassembler.h"
#include "VirtualMachine.h"
#include <iostream>
int main() {

    const std::string program_path = "/home/alexandr/CLionProjects/Assembler/program.asm";
    const std::string bytecode_path = "/home/alexandr/CLionProjects/Assembler/fib.bin";
    const std::string dis_program_path = "/home/alexandr/CLionProjects/Assembler/program.disasm";

    Assembler asm_obj;
    asm_obj.make_bytecode(program_path, bytecode_path);

//    std::cout << "have made bytecode" << std::endl;
    VirtualMachine VM;
    VM.exec_program("/home/alexandr/CLionProjects/Assembler/fib.bin");

    Disassembler disasm_obj;
    disasm_obj.make_program(dis_program_path, bytecode_path);

    return 0;
}