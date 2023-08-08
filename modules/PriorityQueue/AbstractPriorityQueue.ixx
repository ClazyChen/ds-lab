export module PriorityQueue.AbstractPriorityQueue;

import Framework.DataStructure;
import std;

export namespace dslab {

template <typename T>
class AbstractPriorityQueue : public DataStructure<T> {
protected:
    std::function<bool(const T&, const T&)> cmp { std::less<T>() };
public:
    AbstractPriorityQueue() = default;
    AbstractPriorityQueue(const std::function<bool(const T&, const T&)>& cmp) : cmp(cmp) {}
    virtual void push(const T& e) = 0;
    virtual void push(T&& e) = 0;
    virtual T pop() = 0;
    virtual const T& top() const = 0;
    virtual void clear() {
        while (!this->empty()) {
            this->pop();
        }
    }
};

}