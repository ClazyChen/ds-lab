module;
#include <iostream>

export module Deque.LinkedDeque;

import Deque.AbstractDeque;
import LinearList;
import List;

export namespace dslab {

template <typename T, template<typename> typename Lis = List>
    requires std::is_base_of_v<AbstractList<T>, Lis<T>>
class LinkedDeque : public AbstractDeque<T> {
    Lis<T> L;
public:
    void push_back(const T& e) override { L.push_back(e); }
    void push_back(T&& e) override { L.push_back(std::move(e)); }
    void push_front(const T& e) override { L.push_front(e); }
    void push_front(T&& e) override { L.push_front(std::move(e)); }

    T pop_back() override { return L.pop_back(); }
    T pop_front() override { return L.pop_front(); }
    T& back() override { return L.back(); }
    T& front() override { return L.front(); }

    size_t size() const override { return L.size(); }

    template <typename T1, template<typename> typename L1>
    friend std::ostream& operator<<(std::ostream& os, const LinkedDeque<T1, L1>& dq);
};

template <typename T, template<typename> typename Lis>
std::ostream& operator<<(std::ostream& os, const LinkedDeque<T, Lis>& dq) {
    os << "LDEQUE:";
    os << dq.L;
    return os;
}

}