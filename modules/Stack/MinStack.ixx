module;
#include <memory>

export module Stack.MinStack;

import Stack;

export namespace dslab {

template <typename T>
class MinStack : public Stack<T> {
    Stack<T> minStack;
public:
    void push(const T& e) override {
        if (minStack.empty() || e < minStack.top()) {
            minStack.push(e);
        } else {
            minStack.push(minStack.top());
        }
        Stack<T>::push(e);
    }

    void push(T&& e) override {
        if (minStack.empty() || e < minStack.top()) {
            minStack.push(e);
        } else {
            minStack.push(minStack.top());
        }
        Stack<T>::push(std::move(e));
    }

    T pop() override {
        minStack.pop();
        return Stack<T>::pop();
    }

    T& top() override {
        return Stack<T>::top();
    }

    const T& top() const override {
        return Stack<T>::top();
    }

    const T& min() const {
        return minStack.top();
    }
};

}