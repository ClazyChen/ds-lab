export module Deque.AbstractDeque;

import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractDeque : public DataStructure<T> {
public:
    virtual void push_back(const T& e) = 0;
    virtual void push_back(T&& e) = 0;
    virtual void push_front(const T& e) = 0;
    virtual void push_front(T&& e) = 0;
    virtual T pop_back() = 0;
    virtual T pop_front() = 0;
    virtual T& back() = 0;
    virtual T& front() = 0;
    virtual const T& back() const {
        return const_cast<AbstractDeque*>(this)->back();
    }
    virtual const T& front() const {
        return const_cast<AbstractDeque*>(this)->front();
    }
};

}