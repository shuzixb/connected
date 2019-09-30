#ifndef _STACK_H_
#define _STACK_H_

#include<vector>
using namespace std;

template<typename T> class Stack:public vector<T>
{
public:
    void push(T const& e){this->push_back(e);}
    T pop(){ T e=this->back();this->pop_back();return e;}
    T top(){return this->back();}
};

#endif // _STACK_H_
