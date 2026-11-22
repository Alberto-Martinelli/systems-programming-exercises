#include "queue.h"
#include "stack.h"
#include <memory>
#include <stdexcept>

static void require(bool condition) {
    if (!condition) throw std::runtime_error("container behavior check failed");
}

int main() {
    Stack<int> stack;
    const auto& stack_view = stack;
    require(stack_view.empty() && stack_view.size() == 0);
    stack.push(10); stack.push(20);
    require(stack.size() == 2 && stack.pop() == 20 && stack.pop() == 10);
    bool stack_threw = false;
    try { stack.pop(); } catch (const std::out_of_range&) { stack_threw = true; }
    require(stack_threw);

    Queue<int> queue;
    const auto& queue_view = queue;
    require(queue_view.empty() && queue_view.size() == 0);
    queue.enqueue(10); queue.enqueue(20);
    require(queue.size() == 2 && queue.dequeue() == 10 && queue.dequeue() == 20);
    bool queue_threw = false;
    try { queue.dequeue(); } catch (const std::out_of_range&) { queue_threw = true; }
    require(queue_threw);

    Stack<std::unique_ptr<int>> owned_stack;
    owned_stack.push(std::make_unique<int>(7));
    require(*owned_stack.pop() == 7);
    Queue<std::unique_ptr<int>> owned_queue;
    owned_queue.enqueue(std::make_unique<int>(9));
    require(*owned_queue.dequeue() == 9);
}
