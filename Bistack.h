/* 
 * John Deters
 * Bistack
 */

#include <algorithm>
#include <sstream>
#include <string>

#ifndef BISTACK_H
#define	BISTACK_H

// templated class for friend functions
template <typename T>
class BiStack;

// declaration for friend function
template <typename T>
std::ostream& operator<< (std::ostream& out, const BiStack<T> &b);

// templated class
template <typename T>
class BiStack{
    // private members
    private:
        T *stack_;
        T def_;
        size_t size_;
        size_t max_;
        size_t stack1_;
        size_t stack2_;
        size_t top1_;
        size_t top2_;
        // private method
        void grow_and_copy();
    
    // public methods
    public:
        BiStack (size_t max = 32, size_t size = 4);
        BiStack (const BiStack &to_copy);
        ~BiStack ();
        BiStack& operator=(BiStack);
        void push1(T);
        void push2(T);
        bool full();
        T top1();
        T top2();
        void pop1();
        void pop2();
        bool empty1();
        bool empty2();
        
        inline friend std::ostream& operator<< <T> (std::ostream&, const BiStack&);
        //friend inline void operator<< <T>(const BiStack&);
};

// constructor
template <typename T>
BiStack<T>::BiStack(size_t max, size_t size){
    // new array
    stack_ = new T[size];
    size_ = size;
    max_ = max;
    stack1_ = 0;
    stack2_ = size-1;
    top1_ = 0;
    top2_ = size-1;
    // fill array
    std::fill(stack_, stack_+size_, def_);
    
}

// copy
template <typename T>
BiStack<T>::BiStack(const BiStack& to_copy){
    stack_ = new T[to_copy.size_];
    // copy array pointer to new array
    std::copy(to_copy.stack_, to_copy.stack_ + to_copy.size_, stack_);
    size_ = to_copy.size_;
    max_ = to_copy.max_;
    stack1_ = to_copy.stack1_;
    stack2_ = to_copy.stack2_;
    top1_ = to_copy.top1_;
    top2_ = to_copy.top2_;
}

// assign
template <typename T>
BiStack<T>& BiStack<T>::operator =(BiStack<T> assign){
    // swap all elements
    std::swap(this->size_, assign.size_);
    std::swap(this->max_, assign.max_);
    std::swap(this->stack1_, assign.stack1_);
    std::swap(this->stack2_, assign.stack2_);
    std::swap(this->stack_, assign.stack_);
    std::swap(this->top1_, assign.top1_);
    std::swap(this->top2_, assign.top2_);
    return *this;
}

// destructor
template <typename T>
BiStack<T>::~BiStack(){
    delete[] stack_;
}

// transform to print
template <typename T>
std::string print (T elem){
    std::ostringstream out_stream;
    out_stream<<elem;
    return out_stream.str();
}

// print friend function
template <typename T>
inline std::ostream& operator<< (std::ostream& out, const BiStack<T> &b){
    int count = 0;
    out<<"Top 1:"<<b.stack_[b.top1_]<<endl<<"Top 2:"<<b.stack_[b.top2_]<<endl;
    out<<"(bottom)";
    // transform algorithm to print
    std::transform(b.stack_, b.stack_ + b.size_, 
            std::ostream_iterator<std::string>(out, ","), print<T>);
    return out;
}

// push on first stack
template <typename T>
void BiStack<T>::push1(T elem){
    // if stack is full, grow and copy
    if (this->full())
        this->grow_and_copy();
    stack_[stack1_++] = elem;
    top1_ = stack1_-1;
    //cout<<"Stack1_:"<<stack1_<<endl;
}

// push on second stack
template <typename T>
void BiStack<T>::push2(T elem){
    // if stack is full, grow and copy
    if (this->full())
        this->grow_and_copy();
    stack_[stack2_--] = elem;
    top2_ = stack2_+1;
    //cout<<"Stack2_:"<<stack2_<<endl;
}

// is BiStack full
template <typename T>
bool BiStack<T>::full(){
    if (stack2_ == stack1_)
        return true;
    else
        return false;
}

// grow and copy
template <typename T>
void BiStack<T>::grow_and_copy(){
    // Has BiStack grown to capacity
    if (size_ != max_){
        size_t new_size;
        if (size_*2 > max_)
            new_size = max_;
        else
            new_size = size_ * 2;
        // create new array
        T *new_stack = new T [new_size];
        T a;
        std::fill (new_stack, new_stack+new_size, a);
        // copy
        std::copy(stack_, stack_ + stack1_, new_stack);
        std::copy_backward(stack_+stack2_, stack_+size_, new_stack+new_size);
        // and switch
        std::swap(stack_, new_stack);
        // delete old
        delete[]new_stack;
        // update members
        size_ = new_size;
        stack2_ = size_ - stack2_;
        size_t old_top = top2_;
        top2_ = new_size - old_top;
    }
    // throw error if BiStack cannot grow larger
    else
        throw (std::runtime_error("BiStack has reached max capacity and cannot grow."));
}

// top of first stack
template <typename T>
T BiStack<T>::top1(){
    return stack_[top1_];
}

// top of second stack
template <typename T>
T BiStack<T>::top2(){
    return stack_[top2_];
}

// check if stack 1 is empty
template <typename T>
bool BiStack<T>::empty1(){
    if (stack1_ == 0)
        return true;
    else
        return false;
}

// check if stack 2 is empty
template <typename T>
bool BiStack<T>::empty2(){
    if (stack2_ == (size_-1))
        return true;
    else
        return false;
}

// pop top of first
template <typename T>
void BiStack<T>::pop1(){
    if (empty1() == false){
        stack_[top1_] = def_;
        top1_--;
        stack1_--;
    }
    // if empty throw error
    else
        throw (std::runtime_error("Cannot pop because stack is empty"));
}

// pop top of second
template <typename T>
void BiStack<T>::pop2(){
    if (empty2() == false){
        stack_[top2_] = def_;
        top2_++;
        stack2_++;
        
    }
    // if empty throw error
    else
        throw (std::runtime_error("Cannot pop because stack is empty"));
    
}

#endif	/* BISTACK_H */
