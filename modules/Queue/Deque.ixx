module;
#include <ratio>
#include <iostream>
#include <concepts>

export module Deque;

export import Deque.AbstractDeque;
export import Deque.CircularDeque;
export import Deque.LinkedDeque;

import Vector;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class Deque : public AbstractDeque<T> {
    Vec<T> V;
    Rank m_front { 0 };
    void expandDeque() {
        size_t oldSize { V.size() };
        V.resize(std::max(V.capacity() + 1, V.capacity() * 2));
        size_t mid { (V.capacity() + m_front - oldSize) / 2 };
        V.resize(mid + oldSize - m_front);
        std::move_backward(V.begin() + m_front, V.begin() + oldSize, V.end());
        m_front = mid;
    }

public:
    size_t size() const override {
        return V.size() - m_front;
    }

    void push_back(const T& e) override {
        if (V.size() == V.capacity()) {
            expandDeque();
        }
        V.push_back(e);
    }

    void push_back(T&& e) override {
        if (V.size() == V.capacity()) {
            expandDeque();
        }
        V.push_back(std::move(e));
    }

    void push_front(const T& e) override {
        if (m_front == 0) {
            expandDeque();
        }
        V[--m_front] = e;
    }

    void push_front(T&& e) override {
        if (m_front == 0) {
            expandDeque();
        }
        V[--m_front] = std::move(e);
    }

    T pop_back() override {
        return V.pop_back();
    }

    T pop_front() override {
        return V[m_front++];
    }

    T& back() override {
        return V[V.size() - 1];
    }

    T& front() override {
        return V[m_front];
    }

    template <typename T1, template<typename> typename V1>
    friend std::ostream& operator<<(std::ostream& os, const Deque<T1, V1>& dq);
};

template <typename T, template<typename> typename Vec>
std::ostream& operator<<(std::ostream& os, const Deque<T, Vec>& dq) {
    os << "DEQUE:";
    for (size_t i { dq.m_front }; i < dq.V.size(); ++i) {
        os << dq.V[i] << " ";
    }
    return os;
}

}