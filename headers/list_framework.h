#pragma once

#include "linear_framework.h"
#include <iterator>
#include <memory>
using namespace std;

// 这个文件给出了列表相关的基本框架
namespace clazy_framework {

template <typename T> class AbstractListNode; 

// 列表节点的ADT接口
template <typename T>
class AbstractListNode : public enable_shared_from_this<AbstractListNode<T>> {
public:
    // 您需要实现它的直接前驱和直接后继的获取接口和设置接口（返回本身的指针）
    virtual shared_ptr<AbstractListNode<T>> pred() { return this->shared_from_this(); }
    virtual shared_ptr<AbstractListNode<T>> succ() { return this->shared_from_this(); }
    virtual shared_ptr<AbstractListNode<T>> setPred(shared_ptr<AbstractListNode<T>> _pred) { return this->shared_from_this(); }
    virtual shared_ptr<AbstractListNode<T>> setSucc(shared_ptr<AbstractListNode<T>> _succ) { return this->shared_from_this(); }

    // 您需要实现获取数据的接口
    virtual T& data() = 0;
};

// 当您希望构造单链表时，可以用于指定前向指针pred()返回节点本身
// 并在实现的单链表中避免使用前向运算符（-、--、-=）

// 定义位置
template <typename T>
using ListNodePos = shared_ptr<AbstractListNode<T>>;

// 列表的ADT接口
template <typename T>
class AbstractList : public AbstractLinearStructure<T> {
public:
    // 列表的迭代器
    class Iterator {
    protected:
        ListNodePos<T> position;
    public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = ListNodePos<T>;
        using reference         = T&;

        explicit Iterator(ListNodePos<T> position): position(position) {}

        bool operator==(Iterator other) {
            return (position.get() == other.position.get());
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        Iterator& operator++() {
            position = position->succ();
            return *this;
        }

        Iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            position = position->pred();
            return *this;
        }

        Iterator operator--(int) {
            auto temp = *this;
            --(*this);
            return temp;
        }

        Iterator& operator+=(int offset) {
            while (offset-- > 0) {
                ++(*this);
            }
            return *this;
        }

        Iterator operator+(int offset) {
            auto temp = *this;
            temp += offset;
            return temp;
        }

        Iterator& operator-=(int offset) {
            while (offset-- > 0) {
                --(*this);
            }
            return *this;
        }

        Iterator operator-(int offset) {
            auto temp = *this;
            temp -= offset;
            return temp;
        }

        reference operator*() {
            return position->data();
        }

        pointer operator->() {
            return position;
        }

        pointer base() {
            return position;
        }

        reference operator[](int index) {
            return *(*this + index);
        }
    };

    // 您需要获取列表的长度
    virtual int size() const = 0;

    // 您需要定义遍历的起始位置和结束位置
    virtual Iterator begin() const = 0;
    virtual Iterator end() const = 0;

    // 列表的循秩访问，不建议使用
    virtual T& operator[](int r) const {
        return begin()[r];
    }

    // 您需要定义插入元素（包括前插和后插）的接口，返回被插入元素的指针
    virtual ListNodePos<T> insertAsPred(ListNodePos<T> pos, const T& e) = 0;
    virtual ListNodePos<T> insertAsSucc(ListNodePos<T> pos, const T& e) = 0;
    
    // 您需要定义删除元素的接口，返回被删除的元素
    virtual T remove(ListNodePos<T> pos) = 0;

    // 您需要定义查找元素的接口，返回查找到的元素的位置，未找到返回nullptr
    virtual ListNodePos<T> find(const T& e) const = 0;

    // 一些不需要您自己定义的接口
    virtual bool empty() const {
        return begin() == end();
    }

    virtual T& front() const {
        return *begin();
    }

    virtual T& back() const {
        return *(end() - 1);
    }

    virtual void push_front(const T& e) {
        insertAsPred(begin().base(), e);
    }

    virtual void push_back(const T& e) {
        insertAsPred(end().base(), e);
    }

    virtual T pop_front() {
        return remove(begin().base());
    }

    virtual T pop_back() {
        return remove((end() - 1).base());
    }
};

// 您可以实现自己的List，直接继承此模板
// 并实现上述接口，用您的List替代例子中使用的clazy::List
// 从而测试你自己设计的List的正确性

}
