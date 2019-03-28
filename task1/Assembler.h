#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <functional>

class Assembler {
public:
    void make_bytecode(const std::string& program_path, const std::string& bytecode_path);

private:
    static const int mem_size = (1  << 16);
    static const size_t cnt_registers = 8;
    static const int comm_code_size = 18;

    int curr_idx = 0;
    std::string curr_arg;

    int bytecode[mem_size];
    std::ifstream input;

    std::unordered_map<std::string, int> reg_codes;
    std::unordered_map<std::string, int> command_codes;
    std::unordered_map<std::string, std::function<void()>> command_funcs;

    std::unordered_map<std::string, int> function_sp;
    std::unordered_map<std::string, int> jlabel_sp;
    std::unordered_map<std::string, int> data_sp;

    void init_maps();

    void code_mov();
    void code_add();
    void code_sub();
    void code_eq();
    void code_pop();
    void code_push();
    void code_push_ra();
    void code_call();
    void code_def();
    void code_return();
    void code_begin();
    void code_end();
    void code_jlabel();
    void code_jlnz();
    void code_print();
    void code_read();
    void code_prints();
    void code_data();


    void process_str();
    bool is_number(const std::string& str);
    std::pair<bool, int> get_int_or_reg();
    int get_reg();
    int get_jlabel_sp();
    int get_fun_sp();

    void two_operand_comm(int comm_code);
    void one_operand_get_comm(int comm_code);
    void one_operand_put_comm(int comm_code);
    void def_like_comm(int comm_code, std::unordered_map<std::string, int>& such_def_sp_cells);

    void write_byte_code(const std::string& bytecode_path);
};
