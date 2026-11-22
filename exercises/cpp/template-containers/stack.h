#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <vector>

using std::vector;
template <class T>
class lifoStack{
    vector<T> vec;
    public:
        void push(const T elem){
            vec.push_back(elem);
        }

        T pop(){
            T tmp = vec.back();
            vec.pop_back();
            return tmp;
        }

        void visit(){
            std::cout << "LIFO Stack:\n";
            int i;
            for(i=vec.size()-1; i>=0; i--){
                std::cout << vec[i] << "\n";
            }
        }

        int getSize(){
            return vec.size();
        }

        int empty(){
            return vec.size()==0? true : false;
        }
};

#endif