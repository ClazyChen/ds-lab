#pragma once

#include "list_forward.h"
#include "queue_framework.h"

// 基于已有的链表实现队列
// 默认的实现方式应该是使用列表（单向列表即可）的，但也允许您使用向量
// 然而向量无法承担pop_front的巨大开销，所以效率会非常低
// 当然，实际上也可以基于向量做，这种实现我放到了queue.h和queue_circular.h里
// 前者利用搬移，批量地释放掉已出队元素的内存
// 后者利用循环，把已出队元素的内存循环利用起来

namespace clazy {

template <typename T, typename Container = ForwardList<T>>
requires (clazy_framework::is_linear_structure<T, typename Container::Position, Container>)
class LinkedQueue : public clazy_framework::AbstractQueue<T> {
private:
    Container C;
public:
    // 队列的三种基本操作
    void enqueue(const T& e) override { 
        C.push_back(e);
    }
    T dequeue() override {
        return C.pop_front();
    }
    T& front() const override {
        return C.front();
    }

    // 来自通用数据结构类的接口
    void clear() override {
        C.clear();
    }
    int size() const override {
        return C.size();
    }

    // 输出方法声明为友元函数，以便输出队列内的具体内容
    template <typename T1, typename Container1>
    friend ostream& operator<< (ostream& out, const LinkedQueue<T1, Container1>& Q);
};

template <typename T, typename Container>
ostream& operator<< (ostream& out, const LinkedQueue<T, Container>& Q) {
    out << "Queue<-";
    out << Q.C;
    return out << "<-";
}



}
