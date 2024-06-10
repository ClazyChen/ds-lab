#pragma once

#include "Stack.hpp"

namespace dslab::stack {

template <typename T>
class MinStack : public Stack<T> {
protected:
    Stack<T> Sm;
    void update() {
        if (Sm.empty()) {
            Sm.push(this->top());
        } else if (auto t { Sm.top() }; t < this->top()) {
            Sm.push(t);
        } else {
            Sm.push(this->top());
        }
    }
public:
    void push(const T& e) override {
        Stack<T>::push(e);
        update();
    }
    void push(T&& e) override {
        Stack<T>::push(std::move(e));
        update();
    }
    T pop() override {
        Sm.pop();
        return Stack<T>::pop();
    }
    const T& min() const {
        return Sm.top();
    }
    std::string type_name() const override {
        return std::format("Min {}", Sm.type_name());
    }

};

}