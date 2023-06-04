export module Queue.LinkedQueue;

import Queue.AbstractQueue;
import LinearList;
import ForwardList;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear = ForwardList>
    requires std::is_base_of_v<AbstractLinearList<T, typename Linear<T>::position_type>, Linear<T>>
class LinkedQueue : public AbstractQueue<T> {
    Linear<T> L;
public:
    LinkedQueue() = default;

    LinkedQueue(std::initializer_list<T> ilist) : L(ilist) {}
    LinkedQueue& operator=(std::initializer_list<T> ilist) {
        L = ilist;
        return *this;
    }

    void enqueue(const T& e) override {
        L.push_back(e);
    }

    void enqueue(T&& e) override {
        L.push_back(std::move(e));
    }

    T dequeue() override {
        return L.pop_front();
    }

    T& front() override {
        return L.front();
    }

    size_t size() const override {
        return L.size();
    }

    std::string type_name() const override {
        return std::format("Queue ({})", L.type_name());
    }

    template <typename T1, template<typename> typename L1>
    friend std::ostream& operator<<(std::ostream& os, const LinkedQueue<T1, L1>& q);
};

template <typename T, template<typename> typename Linear>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<T, Linear>& q) {
    os << "LQUEUE<-";
    os << q.L;
    os << "<-";
    return os;
}

}