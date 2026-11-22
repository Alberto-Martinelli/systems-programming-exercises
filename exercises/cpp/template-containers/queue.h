#ifndef SYSTEMS_EXERCISES_QUEUE_H
#define SYSTEMS_EXERCISES_QUEUE_H

#include <cstddef>
#include <list>
#include <stdexcept>
#include <utility>

template <typename T>
class Queue {
public:
    void enqueue(T value) { values_.push_back(std::move(value)); }
    T dequeue() {
        if (empty()) throw std::out_of_range("dequeue from empty queue");
        T value = std::move(values_.front());
        values_.pop_front();
        return value;
    }
    std::size_t size() const noexcept { return values_.size(); }
    bool empty() const noexcept { return values_.empty(); }
private:
    std::list<T> values_;
};

#endif
