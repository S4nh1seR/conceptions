#include "Assembler.h"
#include <iostream>

void Assembler::init_maps() {
    reg_codes = {
            {"reg1", 0},
            {"reg2", 1},
            {"reg3", 2},
            {"reg4", 3},
            {"reg5", 4},
            {"reg6", 5},
            {"reg7", 6},
            { "res", 7}
    };
    command_codes = {
            {"mov",     0},
            {"add",     1},
            {"sub",     2},
            {"eq",      3},
            {"pop",     4},
            {"push",    5},
            {"push_ra", 6},
            {"call",    7},
            {"def",     8},
            {"return",  9},
            {"begin",  10},
            {"end",    11},
            {"jlabel", 12},
            {"jlnz",   13},
            {"print",  14},
            {"read",   15},
            {"prints", 16},
            {"data",   17}
    };
    command_funcs = {
            {"mov",     std::bind(&Assembler::code_mov,     this)},
            {"add",     std::bind(&Assembler::code_add,     this)},
            {"sub",     std::bind(&Assembler::code_sub,     this)},
            {"eq",      std::bind(&Assembler::code_eq,      this)},
            {"pop",     std::bind(&Assembler::code_pop,     this)},
            {"push",    std::bind(&Assembler::code_push,    this)},
            {"push_ra", std::bind(&Assembler::code_push_ra, this)},
            {"call",    std::bind(&Assembler::code_call,    this)},
            {"def",     std::bind(&Assembler::code_def,     this)},
            {"return",  std::bind(&Assembler::code_return,  this)},
            {"begin",   std::bind(&Assembler::code_begin,   this)},
            {"end",     std::bind(&Assembler::code_end,     this)},
            {"jlabel",  std::bind(&Assembler::code_jlabel,  this)},
            {"jlnz",    std::bind(&Assembler::code_jlnz,    this)},
            {"print",   std::bind(&Assembler::code_print,   this)},
            {"read",    std::bind(&Assembler::code_read,    this)},
            {"prints",  std::bind(&Assembler::code_prints,  this)},
            {"data",    std::bind(&Assembler::code_data,  this)}
    };
}

void Assembler::make_bytecode(const std::string& program_path, const std::string& bytecode_path) {
    init_maps();
    input = std::ifstream(program_path, std::ios::in);
    curr_idx = cnt_registers + 2;
    bytecode[0] = curr_idx;

    while (input >> curr_arg) {
        command_funcs[curr_arg]();
    }
    bytecode[1] = curr_idx;
    write_byte_code(bytecode_path);
}

void Assembler::two_operand_comm(int comm_code) {
    int dest_reg = get_reg();
    std::pair<bool, int> value = get_int_or_reg();
    bytecode[curr_idx++] = comm_code + value.first * comm_code_size;
    bytecode[curr_idx++] = dest_reg;
    bytecode[curr_idx++] = value.second;
}

void Assembler::code_mov() {
    two_operand_comm(command_codes["mov"]);
}

void Assembler::code_add() {
    two_operand_comm(command_codes["add"]);
}

void Assembler::code_sub() {
    two_operand_comm(command_codes["sub"]);
}

void Assembler::code_eq() {
    two_operand_comm(command_codes["eq"]);
}

void Assembler::one_operand_get_comm(int comm_code) {
    bytecode[curr_idx++] = comm_code;
    bytecode[curr_idx++] = get_reg();
}

void Assembler::code_pop() {
    one_operand_get_comm(command_codes["pop"]);
}

void Assembler::code_read() {
    one_operand_get_comm(command_codes["read"]);
}

void Assembler::one_operand_put_comm(int comm_code) {
    std::pair<bool, int> value = get_int_or_reg();
    bytecode[curr_idx++] = comm_code + value.first * comm_code_size;
    bytecode[curr_idx++] = value.second;
}

void Assembler::code_push() {
    one_operand_put_comm(command_codes["push"]);
}

void Assembler::code_print() {
    one_operand_put_comm(command_codes["print"]);
}

void Assembler::def_like_comm(int comm_code, std::unordered_map<std::string, int>& such_def_sp_cells) {
    input >> curr_arg;
    bytecode[curr_idx++] = comm_code;
    such_def_sp_cells[curr_arg] = curr_idx++;
    process_str();
    bytecode[such_def_sp_cells[curr_arg]] = curr_idx;
}

void Assembler::code_def() {
    def_like_comm(command_codes["def"], function_sp);
}

void Assembler::code_jlabel() {
    def_like_comm(command_codes["jlabel"], jlabel_sp);
}

void Assembler::code_jlnz() {
    one_operand_put_comm(command_codes["jlnz"]);
    bytecode[curr_idx++] = get_jlabel_sp();
}
void Assembler::code_call() {
    bytecode[curr_idx++] = command_codes["call"];
    bytecode[curr_idx++] = get_fun_sp();
}

void Assembler::code_push_ra() {
    bytecode[curr_idx++] = command_codes["push_ra"];
}

void Assembler::code_return() {
    bytecode[curr_idx++] = command_codes["return"];
}

void Assembler::code_begin() {
    bytecode[curr_idx++] = command_codes["begin"];
    bytecode[0] = curr_idx;
}

void Assembler::code_end() {
    bytecode[curr_idx++] = command_codes["end"];
}

void Assembler::code_prints() {
    input.get();
    char first_symbol;
    input.get(first_symbol);
    std::getline(input, curr_arg);

    bool flag_data = (first_symbol == '#');
    bytecode[curr_idx++] = command_codes["prints"] + comm_code_size * flag_data;
    if (flag_data) {
        bytecode[curr_idx++] = data_sp[curr_arg];
    } else {
        std::string tmp = std::string(&first_symbol, 1);
        tmp += curr_arg;
        curr_arg = tmp;
        process_str();
    }
}

void Assembler::process_str() {
    bytecode[curr_idx++] = static_cast<int>(curr_arg.size());
    for (size_t i = 0; i < curr_arg.size(); i += 4) {
        size_t num_chars = (i + 4 > curr_arg.size()) ? curr_arg.size() - i : 4;
        bytecode[curr_idx++] = *reinterpret_cast<const int*>(curr_arg.substr(i, num_chars).c_str());
    }
}

std::pair<bool, int> Assembler::get_int_or_reg() {
    input >> curr_arg;
    bool is_number_flag = is_number(curr_arg);
    int result = is_number_flag ? std::stoi(curr_arg): reg_codes[curr_arg];
    return std::make_pair(is_number_flag, result);
}

int Assembler::get_reg() {
    input >> curr_arg;
    return reg_codes[curr_arg];
}

bool Assembler::is_number(const std::string& str) {
    auto it = str.begin();
    for (; it != str.end() && std::isdigit(*it); ++it) {}
    return !str.empty() && it == str.end();
}
int Assembler::get_jlabel_sp() {
    input >> curr_arg;
    return jlabel_sp[curr_arg];
}

int Assembler::get_fun_sp() {
    input >> curr_arg;
    return function_sp[curr_arg];
}

void Assembler::code_data() {
    input >> curr_arg;
    bytecode[curr_idx++] = command_codes["data"];
    int start_pointer = curr_idx++;
    process_str();
    bytecode[start_pointer] = curr_idx;

    data_sp[curr_arg] = start_pointer;
    input.get();
    std::getline(input, curr_arg);
    process_str();
}

void Assembler::write_byte_code(const std::string& bytecode_path) {
    std::ofstream output(bytecode_path, std::ios::out | std::ios::binary);
    char* text = reinterpret_cast<char*>(bytecode);
    output.write(text, sizeof(int) * mem_size);
}
