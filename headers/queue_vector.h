#pragma once

#include "queue.h"
#include "vector.h"

namespace clazy {

// 和栈不同的是，基于向量去实现队列会发生一个问题
// 就是队列中已经被删除的元素仍然会被保留，一直占据着一部分空间
// 而基于队列去实现的话，则不会有这样的问题
// 所以在使用基于向量的队列的时候，还需要额外规定一个阈值
// 如果队列中的元素数量和向量中的元素数量之比小于一定的比例
// 那么就需要进行搬移，将向量中的元素整体前移
// 和向量的扩容缩容一样，这个搬移的操作的均摊复杂度也是O(1)

template <typename T, typename Container = Vector<T>, int R = 4>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class VectorQueue : public Queue<T, Container> {
protected:
    Rank _front = 0; // 存储队头的秩，相当于Q[d:n]中的d
    void moveElements(); // 搬移元素，当dequeue的时候判断触发

public:
    virtual void clear() override {
        this->C.clear();
        _front = 0; // 清空之后，还需要重置队头的秩
    }
    virtual bool empty() override {
        return this->C.size() == _front; // 判断队列是否为空的条件变为了队头秩是否等于向量大小
    }
    virtual T& front() const override {
        return this->C[_front]; // 返回队头元素
    }
    virtual T dequeue() override {
        T e = this->C[_front++]; // 弹出队头元素
        moveElements(); // 判断是否需要搬移元素
        return e;
    }
    
    VectorQueue() {}
    VectorQueue(const VectorQueue& Q): Queue<T, Container>(Q), _front(Q._front) {}

};

template <typename T, typename Container, int R>
void VectorQueue<T, Container, R>::moveElements() {
    if ((this->C.size() - _front) * R < this->C.size()) { // 达到阈值之后触发搬移
        copy(begin(this->C) + _front, end(this->C), begin(this->C)); // 将向量中的元素前移
        this->C.resize(this->C.size() - _front);          // 将向量的大小减小
        _front = 0;                                       // 重置队头的秩    
    }
}

}