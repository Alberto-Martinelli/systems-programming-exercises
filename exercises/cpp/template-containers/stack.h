#ifndef SYSTEMS_EXERCISES_STACK_H
#define SYSTEMS_EXERCISES_STACK_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class Stack {
public:
    void push(T value) { values_.push_back(std::move(value)); }
    T pop() {
        if (empty()) throw std::out_of_range("pop from empty stack");
        T value = std::move(values_.back());
        values_.pop_back();
        return value;
    }
    std::size_t size() const noexcept { return values_.size(); }
    bool empty() const noexcept { return values_.empty(); }
private:
    std::vector<T> values_;
};

#endif
