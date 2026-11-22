#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <list>

using namespace std;
using std::list;

template <class T>
class fifo_queue{
    list<T> myList;
    public:
        void enqueue(T elem){
            myList.push_back(elem);
        }

        T dequeue(){
            T tmp = myList.front();
            myList.pop_front();
            return tmp;
        }

        void visit(){
            cout << "Printing FIFO list:" << endl;
            for(T elem : myList)
                cout << elem << endl;
        }

        int getSize(){
            return myList.size();
        }

        bool empty(){
            return myList.empty();
        }
};
#endif