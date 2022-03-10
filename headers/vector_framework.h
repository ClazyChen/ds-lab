#pragma once

#include "linear_framework.h"
#include <iterator>
using namespace std;

// 这个文件给出了向量相关的基本框架
namespace clazy_framework {

// 定义秩的概念
using Rank = int;

// 向量的ADT接口
template <typename T>
class AbstractVector : public AbstractLinearStructure<T> {
public:
    // 向量的迭代器；您不需要改动这一部分，它应当是天然有效的
    class Iterator {
    protected:
        T* position;
    public:
        using iterator_category = random_access_iterator_tag; 
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        explicit Iterator(T* position) : position(position) {}

        bool operator==(Iterator other) {
            return (position == other.position);
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        Iterator& operator++() {
            ++position;               // 您需要保证内部元素是连续存储的
            return *this;
        }
        
        Iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            --position;
            return *this;
        }

        Iterator operator--(int) {
            auto temp = *this;
            --(*this);
            return temp;
        }
        
        Iterator& operator+=(int offset) {
            position += offset;
            return *this;
        }

        Iterator operator+(int offset) {
            auto temp = *this;
            temp += offset;
            return temp;
        }

        Iterator& operator-=(int offset) {
            position -= offset;
            return *this;
        }

        Iterator operator-(int offset) {
            auto temp = *this;
            temp -= offset;
            return temp;
        }

        bool operator<(const Iterator& other) const {
            return position < other.position;
        }

        bool operator<=(const Iterator& other) const {
            return position <= other.position;
        }

        bool operator>(const Iterator& other) const {
            return position > other.position;
        }

        bool operator>=(const Iterator& other) const {
            return position >= other.position;
        }

        reference operator*() {
            return *position;
        }

        pointer operator->() {
            return position;
        }

        pointer base() {
            return position;
        }

        reference operator[](int index) {
            return *(position + index);
        }

        difference_type operator-(const Iterator& other) const {
            return position - other.position;
        }
    };

    // 您需要能获取向量的基本属性：容量和规模；并需要能够修改规模
    // 理论上您也可以实现不可扩容的向量（静态顺序表），此时容量是不能被更改的
    virtual int capacity() const = 0;
    virtual int size() const = 0;
    virtual void resize(int size) = 0;

    // 您需要定义遍历的起始位置和结束位置
    virtual Iterator begin() const = 0;
    virtual Iterator end() const = 0;

    // 向量的循秩访问，不允许您自己定义，需要您保证存放的顺序连续性才能生效
    virtual T& operator[](Rank r) const {
        return begin()[r];
    }

    // 您需要定义插入元素的接口，返回被插入元素的秩
    virtual Rank insert(Rank r, const T& e) = 0;
    
    // 您需要定义删除元素的接口，返回被删除的元素
    virtual T remove(Rank r) = 0;

    // 您需要定义查找元素的接口，返回查找到的元素的秩，未找到返回-1
    virtual Rank find(const T& e) const = 0;

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
        insert(0, e);
    }

    virtual void push_back(const T& e) {
        insert(size(), e);
    }

    virtual T pop_front() {
        return remove(0);
    }

    virtual T pop_back() {
        return remove(size() - 1);
    }

};

// 您可以实现自己的Vector，直接继承此模板
// 并实现上述接口，用您的Vector替代例子中使用的clazy::Vector
// 从而测试你自己设计的Vector的正确性

}
