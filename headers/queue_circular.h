#pragma once

#include "queue_vector.h"

namespace clazy {

// 这个文件讨论循环队列的实现
// 循环、搬移和列表，是三种不同的处理队列被删除元素占用空间问题的办法

template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class CircularQueue : public Queue<T, Container> {
protected:
    Rank _front = 0; // 存储队头的秩
    Rank _rear = 0;  // 存储队尾的秩
    void moveElements(); // 搬移元素，不同的是，这个搬移元素是扩容的时候触发的

public:
    virtual void clear() override {
        this->C.clear();
        this->C.resize(this->C.capacity()); // 循环队列中保持填满状态
        _front = 0; // 清空之后，还需要重置队头的秩
        _rear = 0;  // 清空之后，还需要重置队尾的秩
    }
    virtual bool full() const { return (_rear + 1) % this->C.size() == _front; } // 空出一个位置，以区分满和空
    virtual bool empty() const override { return _front == _rear; } // 可以重复利用
    virtual T& front() const override {
        return this->C[_front]; // 返回队头元素
    }
    virtual T dequeue() override {
        T e = this->C[_front]; // 弹出队头元素
        _front = (_front + 1) % this->C.size(); // 队头移动到下一个位置
        return e;
    }
    virtual void enqueue(const T& e) override {
        moveElements();                       // 判断是否需要搬移元素
        this->C[_rear] = e;                   // 插入队尾元素
        _rear = (_rear + 1) % this->C.size(); // 队尾移动到下一个位置
    }
    CircularQueue() { clear(); }
    CircularQueue(const CircularQueue& Q): Queue<T, Container>(Q), _front(Q._front), _rear(Q._rear) {}

    template <typename T1, typename Container1>
    friend ostream& operator<< (ostream& out, const CircularQueue<T1, Container1>& Q);
};

template <typename T, typename Container>
void CircularQueue<T, Container>::moveElements() {
    if (full()) {                           // 如果队列已满，则需要搬移元素
        Rank capacity = this->C.size();
        this->C.resize(capacity + 1);       // 扩容，使用向量自己规定的扩容方式
        this->C.resize(this->C.capacity()); // 扩容之后立刻重置向量的大小保证填满
        if (_rear < _front) {               // 当队尾在队头之前时，需要搬移元素
            copy(begin(this->C), begin(this->C) + _rear, begin(this->C) + capacity); // 把循环队列的元素搬移到新的位置
            _rear += capacity;              // 队尾移动到新的位置    
        }
    }
}

template<typename T, typename Container>
ostream& operator<< (ostream& out, const CircularQueue<T, Container>& Q) {
    out << "CircularQueue[";
    // for (Rank i = Q._front; i != Q._rear; i = (i + 1) % Q.C.size()) {
    //     if (i != Q._front) out << ", ";
    //     out << Q.C[i];
    // }
    out << Q.C;
    return out << "]";
}

}
