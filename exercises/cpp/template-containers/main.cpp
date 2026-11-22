#include "queue.h"
#include "stack.h"
#include <iostream>
#include <string>

int main() {
    Stack<std::string> stack;
    for (const auto* word : {"first", "second", "third"}) stack.push(word);
    std::cout << "stack:";
    while (!stack.empty()) std::cout << ' ' << stack.pop();
    std::cout << '\n';

    Queue<int> queue;
    for (int value : {1, 2, 3}) queue.enqueue(value);
    std::cout << "queue:";
    while (!queue.empty()) std::cout << ' ' << queue.dequeue();
    std::cout << '\n';
}
