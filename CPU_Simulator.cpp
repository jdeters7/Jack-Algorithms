/*
 * John Deters
 * CPU Simulator
 */


#include "cpu_simulator.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

// construct CPU
CPU::CPU(std::initializer_list<Program> p_list){
    lock_flag_ = false;
    program_index_ = 0;
    memory_.clear();
    current_instruction_ = "";
    instruction_fields_.clear();
    // add programs from list to vector
    for (auto item : p_list)
        programs_.push_back(item);
}

// split instructions into pieces
void CPU::split_instruction(){
    //std::cout<<"split_instruction"<<std::endl;
    // reset vector
    instruction_fields_.clear();
    // create stream to get individual items
    std::istringstream iss(current_instruction_);
    // copy items into vector
    std::copy(std::istream_iterator<std::string>(iss), 
            std::istream_iterator<std::string>(), 
            std::back_inserter(instruction_fields_));
    //for (auto i : instruction_fields_)
        //std::cout<<"split: "<< i<<std::endl;
}

// run the CPU
void CPU::run(){
    //std::cout<<"run"<<std::endl;
    // while finished returns true loop programs
    while (finished() != false){
        // get instruction if program is running
        if (programs_[program_index_].get_status() == "running")
            // get instruction
            current_instruction_ = programs_[program_index_].get_instruction(lock_flag_);
        // split and execute if still running
        if (programs_[program_index_].get_status() == "running"){
            split_instruction();
            execute_instruction();
        }
        // next program to index
        program_index_++;
        // reset index if out of size
        if (program_index_ == programs_.size())
            program_index_ = 0;
    }
    //std::cout<<"complete"; 
}

// check if unlocked
bool CPU::is_unlock(){
    // unlocked and lock flag already called
    if ((instruction_fields_[0] == "unlock")&&(lock_flag_ == true)){
        //std::cout<<"is_unlock"<<std::endl;
        return true;
    }
    // error if unlocked called before locked
    else if ((instruction_fields_[0] == "unlock")&&(lock_flag_ == false)){
        programs_[program_index_].set_status("error: unlock called before lock");
        std::cout<<"error: unlock called before lock"<<std::endl;
        return true;
    } 
    // instruction not unlock
    else
        return false;
}

// check if print
bool CPU::is_print(){
    
    // print existing variable
    std::map<std::string,long>::iterator it = memory_.find(instruction_fields_[1]);
    if ((instruction_fields_[0] == "print")&&
            (it != memory_.end())){
        //std::cout<<"is_print"<<std::endl;
        return true;
    }
    
    // variable not in map
    else if(instruction_fields_[0] == "print"){
        programs_[program_index_].set_status("error: variable does not exist");
        std::cout<<"error: variable does not exist"<<std::endl;
        return false;
    }

    // not print instruction
    else
        return false;
}

// check if lock
bool CPU::is_lock(){

    // call lock
    if (instruction_fields_[0] == "lock"){
        //std::cout<<"is_lock"<<std::endl;
        return true;
    }
    // not a lock
    else
        return false;
}

// check if end
bool CPU::is_end(){
    
    // call end
    if (instruction_fields_[0] == "end"){
        //std::cout<<"is_end"<<std::endl;
        return true;
    }
    // not end instruction
    else
        return false;
}

// check if variable assignment
bool CPU::is_assignment(){
    
    // is variable assignment
    if (instruction_fields_[1] == "="){
        //std::cout<<"is_assignment"<<std::endl;
        return true;
    }
    // not variable assignment
    else
        return false;
}

// check if any programs running
bool CPU::finished(){
    
    // flag for any running program
    bool flag = false;
    // double lock call
    bool waiting_flag = true;
    // check status of each program
    for (auto program : programs_){
        if (program.get_status() != "waiting") waiting_flag = false;
        if (program.get_status() == "running") flag = true;
    }
    // double lock error
    if (waiting_flag == true){
        programs_[program_index_].set_status("error: program called twice in same program");
        std::cout<<"error: program called twice in same program"<<std::endl;
    }
    //std::cout<<"finished"<<std::endl;
    return flag;
}

// execute current instruction
void CPU::execute_instruction(){
    
    // end instruction
    if((is_end() == true)||(current_instruction_ == ""))
        programs_[program_index_].set_status("finished");
    // lock instruction
    else if (is_lock() == true)
        lock_flag_ = true;
    // unlock instruction
    else if (is_unlock() == true)
        lock_flag_ = false;
    // variable assignment instruction
    else if (is_assignment() == true)
        memory_[instruction_fields_[0]] = std::stoi(instruction_fields_[2]);
    // print instruction
    else if (is_print() == true)
        std::cout<<memory_.find(instruction_fields_[1])->second<<std::endl;
    // command not accepted
    else if (current_instruction_ != "waiting"){
        std::cout<<"error in command: "<< current_instruction_<<std::endl;
    }
        
}

// Construct Program
Program::Program(std::string file_name){
    status_ = "running";
    instruction_index_ = 0;
    std::ifstream new_file;
    new_file.open(file_name);
    std::string line;
    // get instructions from file
    while (std::getline(new_file, line)){
        instructions_.push_back(line);
    }
    
}

// get instruction
std::string Program::get_instruction(bool lock_flag){
    //std::cout<<"get instruction: ";
    // index out of range
    if (instruction_index_ == instructions_.size()){
        return "";
        set_status("finished");
    }
    // get instruction from vector
    std::string instruction = instructions_[instruction_index_];
    // wait of lock flag is on
    if ((lock_flag == true)&&(instruction == "lock")){
        return"waiting";
    }
    // run instruction
    else{
        instruction_index_ ++;
        //std::cout<<instruction<<std::endl;
        return instruction;
    }
}
