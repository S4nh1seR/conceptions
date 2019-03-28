#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <functional>

class VirtualMachine {
public:
    void exec_program(const std::string& byterun_path);

private:
    static const int comm_code_size = 18;
    static const unsigned res_idx = 9;
    static const int mem_size = (1 << 16);

    int mem[mem_size];
    bool was_end;

    std::function<void()> command_funcs[18] = {
            std::bind(&VirtualMachine::run_mov,     this),
            std::bind(&VirtualMachine::run_add,     this),
            std::bind(&VirtualMachine::run_sub,     this),
            std::bind(&VirtualMachine::run_eq,      this),
            std::bind(&VirtualMachine::run_pop,     this),
            std::bind(&VirtualMachine::run_push,    this),
            std::bind(&VirtualMachine::run_push_ra, this),
            std::bind(&VirtualMachine::run_call,    this),
            std::bind(&VirtualMachine::run_def,     this),
            std::bind(&VirtualMachine::run_return,  this),
            std::bind(&VirtualMachine::run_begin,   this),
            std::bind(&VirtualMachine::run_end,     this),
            std::bind(&VirtualMachine::run_jlabel,  this),
            std::bind(&VirtualMachine::run_jlnz,    this),
            std::bind(&VirtualMachine::run_print,   this),
            std::bind(&VirtualMachine::run_read,    this),
            std::bind(&VirtualMachine::run_prints,  this),
            std::bind(&VirtualMachine::run_data,    this),
    };

    void read_bytecode(const std::string &pathToBinaryFile);
    int get_int_or_reg(int value);

    void run_mov();
    void run_add();
    void run_sub();
    void run_eq();
    void run_pop();
    void run_push();
    void run_push_ra();
    void run_call();
    void run_def();
    void run_return();
    void run_begin();
    void run_end();
    void run_jlabel();
    void run_jlnz();
    void run_print();
    void run_read();
    void run_prints();
    void run_data();

    void process_str(std::string& result);
};
