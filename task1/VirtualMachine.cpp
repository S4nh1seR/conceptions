#include "VirtualMachine.h"

#include <math.h>
#include <iostream>
#include <experimental/filesystem>


void VirtualMachine::exec_program(const std::string& bytecode_path) {
    read_bytecode(bytecode_path);

    was_end = false;
    while (!was_end) {
        command_funcs[mem[mem[0]] % comm_code_size]();
    }
}

void VirtualMachine::read_bytecode(const std::string& bytecode_path) {
    std::ifstream input(bytecode_path, std::ios::in | std::ios::binary);

    size_t cnt_bytes = std::experimental::filesystem::file_size(bytecode_path);
    size_t length = cnt_bytes / 4;

    for(size_t i = 0; i < length; ++i ) {
        input.read(reinterpret_cast<char*>(&(mem[i])), sizeof(int));
    }
    input.close();
}


void VirtualMachine::run_mov() {
    mem[mem[mem[0] + 1] + 2] = get_int_or_reg(mem[mem[0] + 2]);
    mem[0] += 3;
}

void VirtualMachine::run_add() {
    mem[mem[mem[0] + 1] + 2] += get_int_or_reg(mem[mem[0] + 2]);
    mem[0] += 3;
}

void VirtualMachine::run_sub() {
    mem[mem[mem[0] + 1] + 2] -= get_int_or_reg(mem[mem[0] + 2]);
    mem[0] += 3;
}

void VirtualMachine::run_eq() {
    mem[res_idx] = static_cast<int>(mem[mem[mem[0] + 1] + 2] == get_int_or_reg(mem[mem[0] + 2]));
    mem[0] += 3;
}

void VirtualMachine::run_pop() {
    mem[mem[mem[0] + 1] + 2] = mem[mem[1] - 1];
    --mem[1];
    mem[0] += 2;
}

void VirtualMachine::run_push() {
    mem[mem[1]++] = get_int_or_reg(mem[mem[0] + 1]);
    mem[0] += 2;
}

void VirtualMachine::run_push_ra() {
    mem[mem[1]++] = mem[0] + 3;
    mem[0] += 1;
}

void VirtualMachine::run_call() {
    mem[0] = mem[mem[mem[0] + 1]];
}


void VirtualMachine::run_return() {
    mem[0] = mem[res_idx];
}

void VirtualMachine::run_end() {
    was_end = true;
}

void VirtualMachine::run_jlnz() {
    if (get_int_or_reg(mem[mem[0] + 1]) != 0) {
        mem[0] = mem[mem[mem[0] + 2]];
    } else {
        mem[0] += 3;
    }
}

void VirtualMachine::run_print() {
    std::cout << get_int_or_reg(mem[mem[0] + 1]) << std::endl;
    mem[0] += 2;
}

void VirtualMachine::run_read() {
    int number = 0;
    std::cin >> number;
    mem[mem[mem[0] + 1] + 2] = number;
    mem[0] += 2;
}
void VirtualMachine::process_str(std::string& result) {
    int length = mem[mem[0]];
    for (int num_bytes = 0; length > 0; ++num_bytes, length -= 4) {
        int num_chars = (length >= 4) ? 4 : length;
        std::string curr_byte_str(reinterpret_cast<char *>(&mem[mem[0] + 1 + num_bytes]), num_chars);
        result += curr_byte_str;
    }
}

void VirtualMachine::run_prints() {
    bool data_flag = (mem[mem[0]++] >= comm_code_size);
    std::string result = "";
    if (!data_flag) {
        process_str(result);
        int num_bytes = static_cast<int>(ceil(static_cast<double>(mem[mem[0]]) / 4));
        mem[0] += num_bytes + 1;
    } else {
        int return_value = mem[0];
        mem[0] = mem[mem[mem[0]]];
        process_str(result);
        mem[0] = return_value + 1;
    }
    std::cout << result << std::endl;
}


void VirtualMachine::run_def() {
    std::cout << "Error! Start processing def cell!" << std::endl;
}
void VirtualMachine::run_begin() {
    std::cout << "Error! Start processing begin cell!" << std::endl;
}

void VirtualMachine::run_jlabel() {
    std::cout << "Error! Start processing jlabel cell!" << std::endl;
}

void VirtualMachine::run_data() {
    std::cout << "Error! Start processing data cell!" << std::endl;
}

int VirtualMachine::get_int_or_reg(int value) {
    bool is_number = (mem[mem[0]] >= comm_code_size);
    int result = is_number ? value : mem[value + 2];
    return result;
}
