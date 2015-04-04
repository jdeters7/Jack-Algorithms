/*
 * John Deters
 * December 3, 2014
 * Sorted List
 */
#include<iostream>
using std::cout; using std::endl; using std::ostream;
#include<string>
using std::string;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::swap;

// Node Class
template<typename T>
struct Node{
public:
    T data_;
    Node* next_;
    Node() : data_(T()), next_(nullptr) {};
    Node(T d) : data_(d), next_(nullptr) {};
    bool operator<(Node n){return this->data_ < n.data_;}
};

// SortedList declaration
template<typename T>
class SortedList{
private:
    Node<T> *head_;
    Node<T> *tail_;

public:
    SortedList() : head_(nullptr), tail_(nullptr) {};
    SortedList(initializer_list<T>);
    SortedList(SortedList&);
    ~SortedList();
    SortedList& operator=(SortedList s);
    void insert(T);
    Node<T>* find(T val);
    bool del(T);
    size_t length();
    T min();
    T max();
    T mid();

    // inline, easiest for templates
    friend ostream& operator<<(ostream& out, SortedList<T>& sa){
        Node<T> *ptr;
        // iterate and print with comma separation
        for (ptr = sa.head_; ptr != nullptr; ptr = ptr->next_)
            out<<ptr->data_<<",";
        return out;
    }
};

// Constructor
template<typename T>
SortedList<T>::SortedList(initializer_list<T> new_list){
    // initialize head and tail
    head_ = nullptr;
    tail_ = nullptr;
    // loop and insert
    for (auto e : new_list)
        insert(e);
}

// Copy
template <typename T>
SortedList<T>::SortedList(SortedList<T>& s){
    head_ = nullptr;
    tail_ = nullptr;
    // iterate through s and insert its data_ value
    Node<T>* ptr = s.head_;
    while(ptr != nullptr){
        insert(ptr->data_);
        ptr = ptr->next_;
    }
}

// Assign
template <typename T>
SortedList<T>& SortedList<T>::operator =(SortedList<T> s){
    // swap head and tail
    swap(this->head_, s.head_);
    swap(this->tail_, s.tail_);
    return *this;
}

// Destructor
template <typename T>
SortedList<T>::~SortedList(){
    // start with head
    Node<T> *ptr = head_;
    // delete until empty
    while (ptr != nullptr){
        Node<T> * next = ptr->next_;
        delete ptr;
        ptr = next;
    }
        
}

// insert
template<typename T>
void SortedList<T>::insert(T item){
    // make new node, and reference points before and temp
    Node<T> *new_item = new Node<T>(item);
    Node<T> *before = nullptr;
    Node<T> *temp = head_;
    
    // determine position
    while((temp != nullptr) && (*temp<*new_item)){
        before = temp;
        temp = before->next_;
    }

    // insert is new head
    if(before==nullptr){
        new_item->next_ = head_;
        head_ = new_item;
        // if first node, initialize tail
        if (tail_ == nullptr)
            tail_ = head_;
    }
    
    // insert is new tail
    else if(temp==nullptr){
        tail_->next_ = new_item;
        tail_ = new_item;
    }    

    // insert is in middle
    else{
        before->next_ = new_item;
        new_item->next_ = temp;
    }
    
}

// delete operation
template <typename T>
bool SortedList<T>::del(T val){
    // find delete node and have reference to reassign
    Node<T> *to_del = head_;
    Node<T> *before = nullptr;
    
    // find node with equal value
    while(to_del != nullptr){
        if (to_del->data_ == val){
            break;
        }
        before = to_del;
        to_del = to_del->next_;
    }    
    
    // if found
    if (to_del != nullptr){
        // delete and reassign head
        if (before==nullptr)
            head_ = to_del->next_;
        // delete in middle
        else if (before != nullptr){
            before->next_ = to_del->next_;
        }
        // delete and reassign tail
        else
            tail_ = before;
        delete to_del;
        return true;
    }
    
    // not found
    else
        return false;
}

// find
template <typename T>
Node<T>* SortedList<T>::find(T val){
    // iterate with ptr reference
    Node<T>* ptr;
    for (ptr = head_; ptr != nullptr; ptr = ptr->next_)
        // retrun ptr if equal to value
        if (ptr->data_ == val)
                return ptr;
    return ptr;
}

// length
template <typename T>
size_t SortedList<T>::length(){
    // count as iterates
    size_t count = 0;
    for (Node<T>* ptr = head_; ptr != nullptr; ptr = ptr->next_)
        count++;
    return count;
}

// max
template <typename T>
T SortedList<T>::max(){
    // return tail data
    return tail_->data_;
}

// mid
template <typename T>
T SortedList<T>::mid(){
    // get position by dividing length by 2
    int pos = length()/2;
    int count = 0;
    // when position reached return
    Node<T> *ptr = head_;
    while(count < pos){
        ptr = ptr->next_;
        count++;
    }
    return ptr->data_;
}

// min
template <typename T>
T SortedList<T>::min(){
    // return head data
    return head_->data_;
}
