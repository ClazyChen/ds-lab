export module Queue.AbstractQueue;

import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractQueue : public DataStructure<T> {
public:
    virtual void enqueue(const T& e) = 0; // 入队
    virtual void enqueue(T&& e) = 0; // 入队
    virtual T dequeue() = 0; // 出队
    virtual T& front() = 0; // 取队首元素
};

}
