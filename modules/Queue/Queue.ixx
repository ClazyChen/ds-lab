module;
#include <ratio>
#include <iostream>
#include <concepts>

export module Queue;

export import Queue.AbstractQueue;
export import Queue.CircularQueue;
export import Queue.LinkedQueue;

import Vector;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector, typename Q = std::ratio<1, 2>>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>> && (Q::num > 0) && (Q::num <= Q::den)
class Queue : public AbstractQueue<T> {
    Vec<T> V;
    Rank m_front { 0 };
    void moveElements() {
        std::move(V.begin() + m_front, V.end(), V.begin());
        V.resize(V.size() - m_front);
        m_front = 0;
    }
public:
    size_t size() const override {
        return V.size() - m_front;
    }

    void enqueue(const T& e) override {
        V.push_back(e);
    }

    void enqueue(T&& e) override {
        V.push_back(std::move(e));
    }

    T dequeue() override {
        T e { std::move(V[m_front++]) };
        if (m_front >= V.size() * Q::num / Q::den) {
            moveElements();
        }
        return e;
    }

    T& front() override {
        return V[m_front];
    }

    template <typename T1, template<typename> typename V1, typename Q1>
    friend std::ostream& operator<<(std::ostream& os, const Queue<T1, V1, Q1>& q);
};

template <typename T, template<typename> typename Vec, typename Q>
std::ostream& operator<<(std::ostream& os, const Queue<T, Vec, Q>& q) {
    os << "QUEUE<-[";
    for (size_t i { q.m_front }; i < q.V.size(); ++i) {
        os << q.V[i] << ", ";
    }
    os << "]<-";
    return os;
}

}