#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>

class Disassembler {

public:
    void make_program(const std::string& program_path, const std::string& bytecode_path);

private:
    static const int mem_size = (1 << 16);
    static const size_t cnt_registers = 8;
    static const int comm_code_size = 18;
    std::ofstream output;

    int bytecode[mem_size];
    int current = 0;
    bool was_end;

    std::vector<std::string> reg_names;
    std::vector<std::string> command_names;
    std::vector<std::function<void()>> command_funcs;

    std::unordered_map<int, std::string> function_sp;
    std::unordered_map<int, std::string> jlabel_sp;

    void init_vectors();
    void read_bytecode(const std::string& bytecode_path);

    void decode_mov();
    void decode_add();
    void decode_sub();
    void decode_eq();
    void decode_pop();
    void decode_push();
    void decode_push_ra();
    void decode_call();
    void decode_def();
    void decode_return();
    void decode_begin();
    void decode_end();
    void decode_jlabel();
    void decode_jlnz();
    void decode_print();
    void decode_read();
    void decode_prints();
    void decode_data();

    void decode_two_operand_command();
    void decode_one_operand_put_comm();
    void decode_one_operand_get_comm();
    void decode_def_like_comm(std::unordered_map<int, std::string>& such_def_sp_cells);

    std::string get_strval();
    void process_str(std::string& result);
};
