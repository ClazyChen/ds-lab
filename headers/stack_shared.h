#pragma once

#include "vector.h"
#include "stack_framework.h"

namespace clazy {

// 共享栈
template <typename T, typename Container = Vector<T>>
requires (is_base_of_v<clazy_framework::AbstractVector<T>, Container>)
class SharedStack : public clazy_framework::DataStructure<T> {
protected:
    Container V;         // 用一个向量存储两个栈
    Rank topb, topf;     // 两个栈的栈顶位置

    virtual void expand() {      // 共享栈的扩张
        int old_capacity = V.capacity();
        V.resize(V.capacity() + 1); // 先增加一个元素，自动触发扩容
        V.resize(V.capacity());     // 然后将向量填满
        topb += V.capacity() - old_capacity; // 更新反向栈的栈顶位置
        copy_backward(begin(V) + topf, begin(V) + old_capacity, end(V)); // 移动整个反向栈
    }

public:
    virtual bool full() const { return topb + 1 == topf; } // 判断共享栈满
    virtual bool empty() const { return topb == 0 && topf == V.size() - 1; } // 判断共享栈为空
    virtual void clear() {  // 清空共享栈
        V.clear();
        V.resize(V.capacity()); // 向量需要始终保持填满状态，只是其中的元素不一定都会被用到
        topb = 0;
        topf = V.size() - 1;
    }

    class ForwardStack : public clazy_framework::AbstractStack<T> {
    protected:
        const SharedStack* parent; // 依赖于的共享栈
    public:
        virtual void clear() { parent->topf = 0; }
        virtual bool empty() const { return parent->topf == 0; }
        virtual T top() const { return parent->V[parent->topf-1]; }
        virtual T pop() { return parent->V[--parent->topf]; }
        virtual void push(const T& e) {
            if (parent->full()) {
                parent->expand();
            }
            parent->V[parent->topf++] = e;
        }
        ForwardStack(const SharedStack* parent): parent(parent) {}
    };

    class BackwardStack : public clazy_framework::AbstractStack<T> {
    protected:
        const SharedStack* parent;
    public:
        virtual void clear() { parent->topb = V.size() - 1; }
        virtual bool empty() const { return parent->topb == V.size() - 1; }
        virtual T top() const { return parent->V[parent->topb+1]; }
        virtual T pop() { return parent->V[++parent->topb]; }
        virtual void push(const T& e) {
            if (parent->full()) {
                parent->expand();
            }
            parent->V[parent->topb--] = e;
        }
        BackwardStack(const SharedStack* parent): parent(parent) {}
    };

    pair<ForwardStack, BackwardStack> getStacks() const { // 获取两个栈
        return {ForwardStack(this), BackwardStack(this)};
    }

    SharedStack() { clear(); } // 默认构造函数
    SharedStack(const SharedStack<T, Container>& SS): C(SS.C), topb(SS.topb), topf(SS.topf) {} // 复制构造函数


};

}