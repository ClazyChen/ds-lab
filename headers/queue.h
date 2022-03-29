#pragma once

#include "list.h"
#include "queue_framework.h"

namespace clazy {

// 对于一般的队列类，直接使用列表就可以了
template <typename T, typename Container = List<T>>
requires (clazy_framework::is_linear_structure<T, Container>)
class Queue : public clazy_framework::AbstractQueue<T> {
protected:
    Container C;
public:
    virtual void clear() override { C.clear(); } // 清空队列
    virtual bool empty() const override { return C.empty(); } // 判断队列是否为空
    virtual T& front() const override { return C.front(); } // 获取队列头元素
    virtual T dequeue() override { return C.pop_front(); } // 弹出队列头元素并返回
    virtual void enqueue(const T& e) override { C.push_back(e); } // 将元素推入队列
    
    Queue() {} // 默认构造函数
    Queue(const Queue<T, Container>& Q): C(Q.C) {} // 复制构造函数
    Queue(Queue<T, Container>&& Q): C(move(Q.C)) {} // 移动构造函数
    auto& operator=(const Queue<T, Container>& Q) { C = Q.C; return *this; } // 复制赋值
    auto& operator=(Queue<T, Container>&& Q) { C = move(Q.C); return *this; } // 移动赋值
    
    template <typename T1, typename Container1> // 为了能够输出Container的内容，声明输出为友元函数
    friend ostream& operator<< (ostream& out, const Queue<T1, Container1>& Q);
};

template <typename T, typename Container>
ostream& operator<< (ostream& out, const Queue<T, Container>& Q) {
    out << "Queue<-";
    out << Q.C;
    return out << "<-";
}

}