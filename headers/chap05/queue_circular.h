#pragma once

#include "vector.h"
#include "queue_framework.h"

// 这个文件讨论循环队列的实现
// 循环、搬移和列表，是三种不同的处理队列被删除元素占用空间问题的办法
// 循环队列对向量的管理模式和之前的共享栈有相似之处

namespace clazy {

template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class CircularQueue : public Queue<T, Container> {
protected:
    Container V;         // 向量
    Rank _front = 0;     // 存储队头的秩
    Rank _rear = 0;      // 存储队尾的秩
    void moveElements(); // 搬移元素，不同的是，这个搬移元素是扩容的时候触发的

public:
    bool full() const {  // 允许存在1个空位，用来区分空和满两种状态
        return V.size() == 0 || size() + 1 == V.size();
    }

    // 队列的三种基本操作
    void enqueue(const T& e) override {
        if (full()) {
            moveElements();
        }
        V[_rear] = e;
        _rear = (++_rear) % V.size();
    }

    T dequeue() override {
        T e = V[_front];
        _front = (++_front) % V.size();
        return e;
    }

    T& front() const override {
        return V[_front];
    }

    // 来自通用数据结构类的接口
    void clear() override {
        V.clear();
        V.resize(V.capacity()); // 保持填满的状态
        _front = _rear = 0;
    }

    int size() const override {
        if (V.size() == 0) {
            return 0;
        } else {
            return (_rear - _front + V.size()) % V.size();
        }
    }

    CircularQueue() {
        clear();
    }

    // 输出方法声明为友元函数，以便输出队列内的具体内容
    template <typename T1, typename Container1>
    friend ostream& operator<< (ostream& out, const CircularQueue<T1, Container1>& Q);
};

// 进行扩容，此处和共享栈的扩容非常相似
template <typename T, typename Container>
void CircularQueue<T, Container>::moveElements() {
    int old_capacity = V.capacity();
    V.resize(V.capacity() + 1);
    V.resize(V.capacity());
    if (_rear < _front) { // Q[d:n,0:d]移动为Q[d:n+d]
        copy(begin(V), begin(V) + _rear, begin(V) + old_capacity);
        _rear += old_capacity;
    }
}

template <typename T, typename Container>
ostream& operator<< (ostream& out, const CircularQueue<T, Container>& Q) {
    out << "CircularQueue";
    if (Q._rear < Q._front) {
        out << "[";
        for (int i = 0; i < Q._rear; i++) {
            out << Q.V[i] << ", ";
        }
        out << "]<- ";
        if (int step = Q._front - Q._rear - 1; step > 0) {
            out << "...(" << step << ")...";
        }
        out << " <-[";
        for (int i = Q._front; i < Q.V.size(); i++) {
            out << Q.V[i] << ", ";
        }
        out << "]";
    } else {
        out << "<-[";
        for (int i = Q._front; i < Q._rear; i++) {
            out << Q.V[i] << ", ";
        }
        out << "]<-";
    }
    return out;
}

}
