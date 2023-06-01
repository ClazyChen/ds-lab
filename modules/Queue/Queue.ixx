export module Queue;

export import Queue.AbstractQueue;
export import Queue.CircularQueue;
export import Queue.LinkedQueue;

import Vector;
import std;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector, typename Q = std::ratio<3, 4>>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>> && (Q::num > 0) && (Q::num <= Q::den)
class Queue : public AbstractQueue<T> {
    Vec<T> V;
    Rank m_front { 0 };
    void moveElements() {
        std::move(std::begin(V) + m_front, std::end(V), std::begin(V));
        V.resize(V.size() - m_front);
        m_front = 0;
    }
public:
    Queue() = default;

    Queue(std::initializer_list<T> ilist) : V(ilist) {}
    Queue& operator=(std::initializer_list<T> ilist) {
        V = ilist;
        return *this;
    }

    size_t size() const override {
        return V.size() - m_front;
    }

    void enqueue(const T& e) override {
        if (m_front >= V.capacity() * Q::num / Q::den) {
            moveElements();
        }
        V.push_back(e);
    }

    void enqueue(T&& e) override {
        if (m_front >= V.capacity() * Q::num / Q::den) {
            moveElements();
        }
        V.push_back(std::move(e));
    }

    T dequeue() override {
        return std::move(V[m_front++]);
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