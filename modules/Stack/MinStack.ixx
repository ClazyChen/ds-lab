module;
#include <memory>
#include <iostream>

export module Stack.MinStack;

import Stack;

export namespace dslab {

template <typename T>
class MinStack : public Stack<T> {
    Stack<T> minStack;
public:
    void push(const T& e) override {
        Stack<T>::push(e);
        if (minStack.empty()) {
            minStack.push(this->top());
        } else if (auto t { minStack.top() }; t < this->top()) {
            minStack.push(t);
        } else {
            minStack.push(this->top());
        }
    }

    void push(T&& e) override {
        Stack<T>::push(std::move(e));
        if (minStack.empty()) {
            minStack.push(this->top());
        } else if (auto t { minStack.top() }; t < this->top()) {
            minStack.push(t);
        } else {
            minStack.push(this->top());
        }
    }

    T pop() override {
        minStack.pop();
        return Stack<T>::pop();
    }
    
    const T& min() const {
        return minStack.top();
    }
    
    template <typename T1>
    friend std::ostream& operator<<(std::ostream& os, const MinStack<T1>& s);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MinStack<T>& s) {
    os << "STACK[MIN=" << s.minStack << "]-" << s.L << "->";
    return os;
}

}