#ifndef CPU_SIMULATOR
#define CPU_SIMULATOR
#include<vector>
//using std::vector;
#include<map>
//using std::map;
#include<string>
//using std::string;
#include<iostream>
//using std::ostream;
#include<initializer_list>
//using std::initializer_list;


class Program{
private:
    size_t instruction_index_;
    std::vector<std::string> instructions_;
    std::string status_;
public:
    Program(std::string file_name);
    Program() = default;
    std::string get_instruction(bool lock_flag);
    std::string get_status() {return status_;};
    void set_status(std::string s) {status_ = s;} ;
};

class CPU {
private:
    std::string current_instruction_;
    std::vector<std::string> instruction_fields_;
    std::map<std::string,long> memory_;
    std::vector<Program> programs_;
    size_t program_index_;
    bool lock_flag_;
public:
    CPU(std::initializer_list<Program> p_list);
    bool finished();
    void run();
    bool is_assignment();
    bool is_print();
    bool is_end();
    bool is_lock();
    bool is_unlock();
    void split_instruction();
    void execute_instruction();
};

#endif
