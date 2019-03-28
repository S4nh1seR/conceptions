#include "Disassembler.h"

#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

void Disassembler::init_vectors() {
    reg_names = {
            "reg1",
            "reg2",
            "reg3",
            "reg4",
            "reg5",
            "reg6",
            "reg7",
            "res"
    };
    command_names = {
            "mov",
            "add",
            "sub",
            "eq",
            "pop",
            "push",
            "push_ra",
            "call",
            "def",
            "return",
            "begin",
            "end",
            "jlabel",
            "jlnz",
            "print",
            "read",
            "prints",
            "data"
    };
    command_funcs = {
            std::bind(&Disassembler::decode_mov,     this),
            std::bind(&Disassembler::decode_add,     this),
            std::bind(&Disassembler::decode_sub,     this),
            std::bind(&Disassembler::decode_eq,      this),
            std::bind(&Disassembler::decode_pop,     this),
            std::bind(&Disassembler::decode_push,    this),
            std::bind(&Disassembler::decode_push_ra, this),
            std::bind(&Disassembler::decode_call,    this),
            std::bind(&Disassembler::decode_def,     this),
            std::bind(&Disassembler::decode_return,  this),
            std::bind(&Disassembler::decode_begin,   this),
            std::bind(&Disassembler::decode_end,     this),
            std::bind(&Disassembler::decode_jlabel,  this),
            std::bind(&Disassembler::decode_jlnz,    this),
            std::bind(&Disassembler::decode_print,   this),
            std::bind(&Disassembler::decode_read,    this),
            std::bind(&Disassembler::decode_prints,  this),
            std::bind(&Disassembler::decode_data,    this)
    };
}

void Disassembler::make_program(const std::string& program_path, const std::string& bytecode_path) {
    init_vectors();
    read_bytecode(bytecode_path);
    output = std::ofstream(program_path, std::ios::out);
    current = cnt_registers + 2;

    was_end = false;
    while (!was_end) {
        command_funcs[bytecode[current] % comm_code_size]();
    }
}

void Disassembler::read_bytecode(const std::string& bytecode_path) {
    std::ifstream input(bytecode_path, std::ios::in | std::ios::binary);

    size_t cnt_bytes = std::experimental::filesystem::file_size(bytecode_path);
    size_t length = cnt_bytes / 4;

    for(size_t i = 0; i < length; ++i ) {
        input.read(reinterpret_cast<char*>(&(bytecode[i])), sizeof(int));
    }
    input.close();
}

void Disassembler::decode_two_operand_command() {
    std::string value = get_strval();
    output << command_names[bytecode[current] % comm_code_size] << " " << reg_names[bytecode[current + 1]] << " " << value << std::endl;
    current += 3;
}

void Disassembler::decode_mov() {
    decode_two_operand_command();
}

void Disassembler::decode_add() {
    decode_two_operand_command();
}

void Disassembler::decode_sub() {
    decode_two_operand_command();
}

void Disassembler::decode_eq() {
    decode_two_operand_command();
}

void Disassembler::decode_one_operand_get_comm() {
    output << command_names[bytecode[current]] << " " << reg_names[bytecode[current + 1]] << std::endl;
    current += 2;
}

void Disassembler::decode_pop() {
    decode_one_operand_get_comm();
}

void Disassembler::decode_read() {
    decode_one_operand_get_comm();
}

void Disassembler::decode_one_operand_put_comm() {
    std::string value = get_strval();
    output << command_names[bytecode[current] % comm_code_size] << " " << value << std::endl;
    current += 2;
}

void Disassembler::decode_print() {
    decode_one_operand_put_comm();
}

void Disassembler::decode_push() {
    decode_one_operand_put_comm();
}

void Disassembler::decode_push_ra() {
    output << command_names[bytecode[current++]] << std::endl;
}

void Disassembler::decode_call() {
    output << command_names[bytecode[current]] << " " << function_sp[bytecode[current + 1]] << std::endl;
    current += 2;

}

void Disassembler::process_str(std::string& result) {
    int length = bytecode[current];
    for (int num_bytes = 0; length > 0; ++num_bytes, length -= 4) {
        int num_chars = (length >= 4) ? 4 : length;
        std::string curr_byte_str(reinterpret_cast<char*>(&bytecode[current + 1 + num_bytes]), num_chars);
        result += curr_byte_str;
    }
}

void Disassembler::decode_def_like_comm(std::unordered_map<int, std::string>& such_def_sp_cells) {
    current += 2;
    std::string result;
    process_str(result);
    such_def_sp_cells[current - 1] = result;
    output << command_names[bytecode[current - 2]] << " " << result << std::endl;
    current += static_cast<int>(ceil(static_cast<double>(result.size()) / 4)) + 1;
}

void Disassembler::decode_def() {
    decode_def_like_comm(function_sp);
}

void Disassembler::decode_jlabel() {
    decode_def_like_comm(jlabel_sp);
}

void Disassembler::decode_return() {
    output << command_names[bytecode[current++]] << std::endl;
}

void Disassembler::decode_begin() {
    output << command_names[bytecode[current++]] << std::endl;
}

void Disassembler::decode_end() {
    output << command_names[bytecode[current]] << std::endl;
    was_end = true;
}

void Disassembler::decode_jlnz() {
    std::string value = get_strval();
    output << command_names[bytecode[current] % comm_code_size] << " " << value << " " << jlabel_sp[bytecode[current + 2]] << std::endl;
    current += 3;
}


void Disassembler::decode_prints() {
    output << command_names[bytecode[current] % comm_code_size] << " ";

    bool data_flag = (bytecode[current++] >= comm_code_size);
    std::string result = "";
    if (!data_flag) {
        process_str(result);
        int num_bytes = static_cast<int>(ceil(static_cast<double>(bytecode[current]) / 4));
        current += num_bytes + 1;
    } else {
        int return_value = current;
        current = bytecode[current] + 1;
        process_str(result);
        current = return_value + 1;
        output << '#';
    }
    output << result << std::endl;
}

void Disassembler::decode_data() {
    output << command_names[bytecode[current]] << " ";
    current += 2;

    std::string result;
    process_str(result);
    output << result << " ";
    int num_bytes = static_cast<int>(ceil(static_cast<double>(bytecode[current]) / 4));

    current += num_bytes + 1;
    result.clear();
    process_str(result);
    output << result << std::endl;
    num_bytes = static_cast<int>(ceil(static_cast<double>(bytecode[current]) / 4));

    current += num_bytes + 1;
}

std::string Disassembler::get_strval() {
    bool is_number = (bytecode[current] >= comm_code_size);
    std::string value = is_number ? std::to_string(bytecode[current + 1]) : reg_names[bytecode[current + 1]];
    return value;
}