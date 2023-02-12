module;
#include <iostream>
#include <concepts>

export module Stack;

export import Stack.AbstractStack;
export import Stack.RandomStackOperation;
export import Stack.SharedStack;

import Vector;
import LinearList;

export namespace dslab {

template <typename T, template<typename> typename Linear = DefaultVector>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class Stack : public AbstractStack<T> {
    Linear<T> L;
public:
    constexpr static bool is_vector = std::is_base_of_v<AbstractVector<T>, Linear<T>>;

    Stack() = default;
    Stack(const T& e) : L({ e }) {}
    Stack(T&& e) : L({ std::move(e) }) {}

    Stack(std::initializer_list<T> ilist) : L(ilist) {}
    Stack& operator=(std::initializer_list<T> ilist) {
        L = ilist;
        return *this;
    }

    void push(const T& e) override {
        if constexpr (is_vector) {
            L.push_back(e);
        } else {
            L.push_front(e);
        }
    }

    void push(T&& e) override {
        if constexpr (is_vector) {
            L.push_back(std::move(e));
        } else {
            L.push_front(std::move(e));
        }
    }

    T pop() override {
        if constexpr (is_vector) {
            return L.pop_back();
        } else {
            return L.pop_front();
        }
    }

    T& top() override {
        if constexpr (is_vector) {
            return L.back();
        } else {
            return L.front();
        }
    }

    size_t size() const override { return L.size(); }

    template <typename T1, template<typename> typename L1>
    friend std::ostream& operator<<(std::ostream& os, const Stack<T1, L1>& s);
};

template <typename T, template<typename> typename Linear>
std::ostream& operator<<(std::ostream& os, const Stack<T, Linear>& s) {
    os << "STACK-" << s.L << "->";
    return os;
}

}