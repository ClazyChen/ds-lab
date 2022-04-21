#pragma once

#include "vector.h"
#include "queue_framework.h"

// 和栈不同的是，基于向量去实现队列会发生一个问题
// 就是队列中已经被删除的元素仍然会被保留，一直占据着一部分空间
// 而基于列表去实现的话，则不会有这样的问题
// 所以在使用基于向量的队列的时候，还需要额外规定一个阈值
// 如果队列中的元素数量和向量中的元素数量之比小于一定的比例
// 那么就需要进行搬移，将向量中的元素整体前移
// 和向量的扩容缩容一样，这个搬移的操作的均摊复杂度也是O(1)

namespace clazy {

// 在这个文件中，将给出一个基于向量实现队列的例子
// 参数R用来表示阈值，当向量中的元素数量：队列中的元素数量到达这个阈值时，触发搬移
template <typename T, typename Container = Vector<T>, int R = 2>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class Queue : public clazy_framework::AbstractQueue<T> {
private:
    Container V;         // 向量
    Rank _front;         // 队头的秩，相当于Q[d:n]中的d
    void moveElements(); // 搬移元素，当队列中已出队的元素数量过多时，触发搬移

public:
    // 队列的三种基本操作
    void enqueue(const T& e) override {
        V.push_back(e);
    }

    T dequeue() override {
        T e = V[_front++];
        if (V.size() > R * size()) { // 到达阈值时触发搬移
            moveElements();
        }
        return e;
    }

    T& front() const override {
        return V[_front];
    }

    // 来自通用数据结构类的接口
    void clear() override {
        V.clear();
        _front = 0;
    }

    int size() const override {
        return V.size() - _front;
    }

    // 输出方法声明为友元函数，以便输出队列内的具体内容
    template <typename T1, typename Container1, int R1>
    friend ostream& operator<< (ostream& out, const Queue<T1, Container1, R1>& Q);
};

template <typename T, typename Container, int R>
void Queue<T, Container, R>::moveElements() {
    copy(V.begin() + _front, V.end(), V.begin()); // 将向量中的元素前移
    V.resize(size());                          // 重置向量的规模
    _front = 0;                                // 将队头移到向量的开头
}

template <typename T, typename Container, int R>
ostream& operator<< (ostream& out, const Queue<T, Container, R>& Q) {
    out << "Queue-{";
    for (int i = 0; i < Q._front; i++) {
        out << Q.V[i] << ", ";
    }
    out << "}<-[";
    for (int i = Q._front; i < Q.V.size(); i++) {
        out << Q.V[i] << ", ";
    }
    out << "]<-";
    return out;
}


}