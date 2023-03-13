module;
#include <iostream>

export module Deque.CircularDeque;

import Deque.AbstractDeque;
import Vector;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class CircularDeque : public AbstractDeque<T> {
    Vec<T> V;
    size_t m_front { 0 };
    size_t m_size { 0 };
    bool full() {
        return m_size == V.capacity();
    }
    void expandDeque() {
        V.resize(std::max(V.capacity() + 1, V.capacity() * 2));
        std::move_backward(std::begin(V) + m_front, std::begin(V) + m_front + m_size, std::end(V));
    }
public:
    CircularDeque() = default;

    CircularDeque(std::initializer_list<T> ilist) : V(ilist), m_size { ilist.size() } {}
    CircularDeque& operator=(std::initializer_list<T> ilist) {
        V = ilist;
        m_size = ilist.size();
        return *this;
    }

    void push_back(const T& e) override {
        if (full()) {
            expandDeque();
        }
        V[(m_front + m_size) % V.size()] = e;
        ++m_size;
    }

    void push_back(T&& e) override {
        if (full()) {
            expandDeque();
        }
        V[(m_front + m_size) % V.size()] = std::move(e);
        ++m_size;
    }

    void push_front(const T& e) override {
        if (full()) {
            expandDeque();
        }
        m_front = (m_front + V.size() - 1) % V.size();
        V[m_front] = e;
        ++m_size;
    }

    void push_front(T&& e) override {
        if (full()) {
            expandDeque();
        }
        m_front = (m_front + V.size() - 1) % V.size();
        V[m_front] = std::move(e);
        ++m_size;
    }

    T pop_back() override {
        T e { std::move(V[(m_front + m_size - 1) % V.size()]) };
        --m_size;
        return e;
    }

    T pop_front() override {
        T e { std::move(V[m_front]) };
        m_front = (m_front + 1) % V.size();
        --m_size;
        return e;
    }

    T& front() override {
        return V[m_front];
    }

    T& back() override {
        return V[(m_front + m_size - 1) % V.size()];
    }

    size_t size() const override {
        return m_size;
    }

    template <typename T1, template<typename> typename V1>
    friend std::ostream& operator<<(std::ostream& os, const CircularDeque<T1, V1>& dq);
};

template <typename T, template<typename> typename Vec>
std::ostream& operator<<(std::ostream& os, const CircularDeque<T, Vec>& dq) {
    os << "CIRCULAR-DEQUE:";
    for (size_t i { dq.m_front }; i < dq.m_front + dq.m_size; ++i) {
        os << dq.V[i % dq.V.size()] << ", ";
    }
    return os;
}

}
