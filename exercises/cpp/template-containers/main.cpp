#include "stack.h"
#include "queue.h"
#include <string>

using namespace std;
using std::vector;

int main(int argc, char** argv){
    //TESTING STACK
    lifoStack<string> my_stack;
    my_stack.push("ciao");
    my_stack.push("io sono");
    my_stack.push("Alberto");
    my_stack.visit();
    my_stack.pop();
    my_stack.visit();
    
    std::cout << "\nSize: " <<my_stack.getSize();

    cout << "\nIs the stack empty? ";
    if(my_stack.empty()==1)
        cout << "Yes";
    else
        cout << "No";
    
    //TESTING QUEUE
    fifo_queue<int> my_queue;
    my_queue.enqueue(1);
    my_queue.enqueue(29);
    my_queue.enqueue(83);
    my_queue.visit();
    my_queue.dequeue();
    my_queue.visit();
    cout << "\nSize: " << my_queue.getSize();
    cout << "\nIs the queue empty?" << my_queue.empty();

    return 0;
}