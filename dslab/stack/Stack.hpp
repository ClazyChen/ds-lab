#pragma once

#include "AbstractStack.hpp"
#include "../vector.hpp"

namespace dslab::stack {

template <typename T, template<typename> typename L = DefaultVector>
requires std::is_base_of_v<LinearList<typename L<T>::value_type, typename L<T>::iterator, typename L<T>::const_iterator>, L<T>>
class Stack : public AbstractStack<T> {
protected:
    L<T> V;
    friend class std::formatter<T, Stack<T, L>>;
public:
    Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) noexcept = default;

    Stack(std::initializer_list<T> ilist) : V(ilist) {}
    Stack& operator=(std::initializer_list<T> ilist) {
        V = ilist;
        return *this;
    }

    void push(const T& e) override {
        V.push_back(e);
    }

    void push(T&& e) override {
        V.push_back(std::move(e));
    }

    T pop() override {
        return V.pop_back();
    }

    T& top() override {
        return V.back();
    }

    const T& top() const {
        return V.back();
    }

    std::size_t size() const override {
        return V.size();
    }

    virtual ~Stack() = default;

    void clear() override {
        V.clear();
    }

    std::string type_name() const override {
        return std::format("Stack<{}>", V.type_name());
    }
};

}