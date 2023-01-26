module;
#include <iostream>

export module Queue.CircularQueue;

import Queue.AbstractQueue;
import Vector;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class CircularQueue : public AbstractQueue<T> {
    Vec<T> V;
    size_t m_front { 0 };
    size_t m_size { 0 };
    bool full() {
        return m_size == V.capacity();
    }
    void expandQueue() {
        size_t oldSize { V.size() };
        V.resize(std::max(V.capacity() + 1, V.capacity() * 2));
        std::move(V.begin(), V.begin() + m_front, V.begin() + oldSize);
    }

public:
    void enqueue(const T& e) override {
        if (full()) {
            expandQueue();
        }
        V[(m_front + m_size) % V.size()] = e;
        ++m_size;
    }

    void enqueue(T&& e) override {
        if (full()) {
            expandQueue();
        }
        V[(m_front + m_size) % V.size()] = std::move(e);
        ++m_size;
    }

    T dequeue() override {
        T e { std::move(V[m_front]) };
        m_front = (m_front + 1) % V.size();
        --m_size;
        return e;
    }

    T& front() override {
        return V[m_front];
    }

    size_t size() const override {
        return m_size;
    }

    template <typename T1, template<typename> typename V1>
    friend std::ostream& operator<<(std::ostream& os, const CircularQueue<T1, V1>& q);
};

template <typename T, template<typename> typename Vec>
std::ostream& operator<<(std::ostream& os, const CircularQueue<T, Vec>& q) {
    os << "CIRCULAR-QUEUE<-[";
    for (size_t i { q.m_front }; i < q.m_front + q.m_size; ++i) {
        os << q.V[i % q.V.size()] << ", ";
    }
    os << "]<-";
    return os;
};

}