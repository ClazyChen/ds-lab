#pragma once

// 这个文件给了共享栈的一个参考实现，它基于向量，但不使用向量的方法管理内存
// 只是使用向量的方法进行扩容

#include "vector.h"
#include "stack_shared_framework.h"

namespace clazy {

template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class SharedStack : public clazy_framework::AbstractSharedStack<T> {
private:
    Container V;  // 用一个向量存储两个栈
    Rank topb, topf; // 两个栈的栈顶位置\

    void expand();   // 共享栈的扩张

public:
    // 分别定义两个栈的类
    class BackwardStack : public clazy_framework::AbstractStack<T> {
    private:
        SharedStack& parent;
    public:
        virtual void clear() override { parent.topb = parent.V.size() - 1; }
        virtual int size() const override { return parent.V.size() - 1 - parent.topb; }
        virtual T& top() const override { return parent.V[parent.topb+1]; }
        virtual T pop() override { return parent.V[++parent.topb]; }
        virtual void push(const T& e) override {
            if (parent.full()) {
                parent.expand();
            }
            parent.V[parent.topb--] = e;
        }
        BackwardStack(SharedStack& parent): parent(parent) {}
    };
    
    class ForwardStack : public clazy_framework::AbstractStack<T> {
    private:
        SharedStack& parent;
    public:        
        virtual void clear() override { parent.topf = 0; }
        virtual int size() const override { return parent.topf; }
        virtual T& top() const override { return parent.V[parent.topf-1]; }
        virtual T pop() override { return parent.V[--parent.topf]; }
        virtual void push(const T& e) override {
            if (parent.full()) {
                parent.expand();
            }
            parent.V[parent.topf++] = e;
        }
        ForwardStack(SharedStack& parent): parent(parent) {}
    };

    // 共享栈
    BackwardStack Sb;
    ForwardStack Sf;

public:
    virtual bool full() const override {
        return topb + 1 == topf;
    }
    virtual int size() const override {
        return Sb.size() + Sf.size();  
    }
    virtual void clear() override{
        V.clear();
        V.resize(V.capacity()); // 保证向量被填满，如果用的我的参考实现，则不需要这一步
        Sb.clear();
        Sf.clear();
    }
    virtual pair<clazy_framework::AbstractStack<T>&, clazy_framework::AbstractStack<T>&> getStacks() override {
        return {Sb, Sf};
    }

    SharedStack(): Sb(*this), Sf(*this) {
        clear();
    }

    template <typename T1, typename Container1> // 为了能够输出Container的内容，声明输出为友元函数
    friend ostream& operator<< (ostream& out, const SharedStack<T1, Container1>& S);

};

// 这里沿用Vector的扩容模式，不同的是，这里对内部存储的管理是使用两个top自己管理的
// 所以V的size没有用了，我让它始终等于capacity
// 在共享栈需要扩张的时候：
// 1. 增加一个元素，触发Vector的扩容
// 2. 将V的size扩张到capacity
// 3. Sf不需要移动，但Sb需要整体向后移动
// 在这个过程中会损失一些效率，因为Sb被重复赋值了
// 向量扩容的时候移动过一次，后面又需要向后移动一次
template <typename T, typename Container>
void SharedStack<T, Container>::expand() {
    int old_capacity = V.capacity();
    V.resize(V.capacity() + 1);
    V.resize(V.capacity());
    copy_backward(begin(V) + topb + 1, begin(V) + old_capacity, end(V)); // 整体移动整个反向栈
    topb += V.capacity() - old_capacity; // 更新反向栈的栈顶
}

template <typename T, typename Container>
ostream& operator<< (ostream& out, const SharedStack<T, Container>& S) {
    out << "Stack(shared)[";
    for (int i = 0; i < S.topf; i++) {
        out << S.V[i] << ", ";
    }
    out << "-> "; 
    if (int step = S.topb - S.topf + 1; step > 0) {
        out << "...(" << step << ")...";
    }
    out << " <-";
    for (int i = S.topb + 1; i < S.V.size(); i++) {
        out << ", " << S.V[i];
    }
    out << "]";
    return out;
}

}
