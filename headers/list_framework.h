#pragma once

#include "linear_framework.h"
#include <iterator>
using namespace std;

// 这个文件给出了列表相关的基本框架
namespace clazy_framework {

// 在这个实现里，Node需要静态成员函数constexpr bool isBidirectional
// 好像静态函数不能被设置为constexpr 所以这里实现很粗暴，只能用concept强制您实现这个函数
// 这个地方我暂时不知道什么方法可以更优雅一点判断一个量是constexpr，所以用了array的大小来做
// 同时，需要Node接受无参数（用于初始化）和带参数的两种构造函数
template <typename T, typename Node>
concept is_listnode_type = requires (const T& e){
    { Node::isBidirectional() } -> same_as<bool>;
    Node();
    Node(e);
    array<char, Node::isBidirectional()> {};
};

// 列表的ADT接口
// 为了实现静态链表，这里除了元素类型T之外，另外接受一个模板参数P表示“位置的表示方法”
// 在通常的列表中，位置使用指针即ListNodePos<T>实现
// 而在静态的列表中，位置使用偏移量Rank实现
// 第三个参数指示，是否是双向链表
template <typename T, typename P, typename Node>
requires (is_listnode_type<T, Node>)
class AbstractList : public AbstractLinearStructure<T> {
public:
    // 您需要定义按位置获取数据，及其在线性列表中的前驱和后继
    virtual T& data(P pos) const = 0;
    virtual P pred(P pos) const = 0;
    virtual P succ(P pos) const = 0;
    // 设置前驱和后继，然后返回pos本身（如果pos无效则什么都不做）
    virtual P setPred(P pos, P _pred) = 0;
    virtual P setSucc(P pos, P _succ) = 0;
    // 产生一个无效的位置（nullptr，静态列表通常可取-1）
    virtual P invalid() const = 0;

    // 列表的迭代器
    // 注意，迭代器的base是P类型的，也就是说在静态链表中是Rank
    class Iterator {
    protected:
        P position;
        const AbstractList<T, P, Node>& L;
    public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = P;
        using reference         = T&;

        explicit Iterator(P position, const AbstractList<T, P, Node>& L): position(position), L(L) {}

        bool operator==(Iterator other) {
            return (position == other.position);
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        Iterator& operator++() {
            position = L.succ(position);
            return *this;
        }

        Iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            position = L.pred(position);
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
            return L.data(position);
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

    // 您需要定义插入元素（包括前插和后插）的接口，返回被插入元素的位置
    virtual P insertAsPred(P pos, const T& e) = 0;
    virtual P insertAsSucc(P pos, const T& e) = 0;
    
    // 您需要定义删除元素的接口，返回被删除的元素
    virtual T remove(P pos) = 0;

    // 您需要定义查找元素的接口，返回查找到的元素的位置，未找到返回nullptr
    virtual P find(const T& e) const = 0;

    // 一些不需要您自己定义的接口
    virtual bool empty() const override {
        return begin() == end();
    }

    virtual T& front() const override {
        return *begin();
    }

    virtual T& back() const override {
        if constexpr (Node::isBidirectional()) {
            return *(end() - 1);
        } else /* forward list */ {
            return *(begin() + (size() - 1));
        }
    }

    virtual void push_front(const T& e) override {
        insertAsPred(begin().base(), e);
    }

    virtual void push_back(const T& e) override {
        insertAsPred(end().base(), e);
    }

    virtual T pop_front() override {
        return remove(begin().base());
    }

    virtual T pop_back() override {
        if constexpr (Node::isBidirectional()) {
            return remove((end() - 1).base());
        } else /* forward list */ {
            return remove((begin() + (size() - 1)).base());
        }
    }
};

// 您可以实现自己的List，直接继承此模板
// 并实现上述接口，用您的List替代例子中使用的clazy::List
// 从而测试你自己设计的List的正确性

}
