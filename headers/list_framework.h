#pragma once

#include "linear_framework.h"
#include <iterator>
using namespace std;

// 这个文件给出了列表相关的基本框架
namespace clazy_framework {
    
// 列表节点的ADT接口
template <typename T>
class AbstractListNode {
protected:
    T _data;
public:
    // 您需要实现它的直接前驱和直接后继的获取接口和设置接口（返回本身的指针）
    virtual AbstractListNode<T>* pred() { return this; }
    virtual AbstractListNode<T>* succ() { return this; }
    virtual AbstractListNode<T>* setPred(AbstractListNode<T>* _pred) { return this; }
    virtual AbstractListNode<T>* setSucc(AbstractListNode<T>* _succ) { return this; }

    // 您需要实现获取数据的接口
    virtual T& data() { return _data; }

    // 默认实现的构造函数
    AbstractListNode() {}
    AbstractListNode(T _data): _data(_data) {}

    // 默认实现的比较接口
    bool operator==(const AbstractListNode<T>& other) const { return _data == other._data; };
    bool operator!=(const AbstractListNode<T>& other) const { return !(*this == other);}
};

// 当您希望构造单链表时，可以用于指定前向指针pred()返回节点本身
// 并在实现的单链表中避免使用前向运算符（-、--、-=）

// 在这个实现里，Node需要静态成员函数constexpr bool isBidirectional
// 好像静态函数不能被设置为constexpr 所以这里实现很粗暴，只能用concept强制您实现这个函数
// 这个地方我暂时不知道什么方法可以更优雅一点判断一个量是constexpr，所以用了array的大小来做
template <typename Node>
concept DirectionDefined = requires {
    { Node::isBidirectional() } -> same_as<bool>;
    array<char, Node::isBidirectional()> {};
};

// 定义一个链表节点需要满足的条件：继承于抽象链表类，并且定义了constexpr的方向
// 实际上作为一个静态节点，还必须满足移动时指针不发生混乱，这个大概需要用static assert弄，暂时先不写了
template <typename T, typename Node>
concept ListNodeType = is_base_of_v<clazy_framework::AbstractListNode<T>, Node> && DirectionDefined<Node>;

// 定义位置
template <typename T>
using ListNodePos = AbstractListNode<T>*;

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
            return (position == other.position);
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

    // 特别提醒：单链表无法使用，需要重载
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

    // 特别提醒：单链表无法使用，需要重载
    virtual T pop_back() {
        return remove((end() - 1).base());
    }
};

// 您可以实现自己的List，直接继承此模板
// 并实现上述接口，用您的List替代例子中使用的clazy::List
// 从而测试你自己设计的List的正确性

}
