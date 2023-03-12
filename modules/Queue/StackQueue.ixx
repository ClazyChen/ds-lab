module;
#include <iostream>

export module Queue.StackQueue;

import Queue.AbstractQueue;
import Stack.SharedStack;
import Vector;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class StackQueue : public AbstractQueue<T> {
    SharedStack<T, Vec> SS;
    auto& backStack() {
        return SS.getStacks().second;
    }
    auto& frontStack() {
        return SS.getStacks().first;
    }

public:
    void enqueue(const T& e) override {
        backStack().push(e);
    }
    void enqueue(T&& e) override {
        backStack().push(std::move(e));
    }
    T dequeue() override {
        if (frontStack().empty()) {
            while (!backStack().empty()) {
                frontStack().push(backStack().pop());
            }
        }
        return frontStack().pop();
    }
    T& front() override {
        if (frontStack().empty()) {
            while (!backStack().empty()) {
                frontStack().push(backStack().pop());
            }
        }
        return frontStack().top();
    }
    size_t size() const override {
        return SS.size();
    }

    template <typename T1, template<typename> typename V1>
    friend std::ostream& operator<<(std::ostream& os, const StackQueue<T1, V1>& q);

};

template <typename T, template<typename> typename Vec>
std::ostream& operator<<(std::ostream& os, const StackQueue<T, Vec>& q) {
    os << "QUEUE(" << q.SS << ")";
}

}