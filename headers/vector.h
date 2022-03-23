#pragma once

#include "vector_framework.h"
#include "allocator.h"

namespace clazy {
using Rank = clazy_framework::Rank;

// 这个文件定义了一个基本的向量

// 默认的向量容量变化策略
// 加倍扩容，永不缩容

using DefaultVectorAllocator = RatioAllocator<2, 1>;

template <typename T>
using VectorIterator = clazy_framework::AbstractVector<T>::Iterator;

// 向量
template <typename T, typename Allocator = DefaultVectorAllocator>
requires (is_base_of_v<clazy_framework::AbstractAllocator, Allocator>)
class Vector : public clazy_framework::AbstractVector<T> {
protected:
    T* _data;                // 向量的数据区
    int _capacity;           // 向量的容量
    int _size;               // 向量的规模

    // 向量扩容缩容的策略
    static clazy_framework::AbstractAllocator* allocator;

    // 控制容量的函数
    virtual void setCapacity(int capacity);

public:
    Vector();                   // 默认构造函数，生成空向量
    Vector(int capacity);       // 生成指定规模的空向量
    Vector(const Vector<T, Allocator>& V); // 复制构造函数，复制向量
    ~Vector() { delete[] _data; } // 析构函数

    // 实现获取容量和规模、修改规模的函数
    virtual int capacity() const { return _capacity; }
    virtual int size() const { return _size; }
    virtual void resize(int size);
    virtual void clear() { resize(0); }

    // 实现获取起始位置和结束位置的函数
    virtual VectorIterator<T> begin() const { return VectorIterator<T>(_data); }
    virtual VectorIterator<T> end() const { return begin() + _size; }

    // 插入元素，返回被插入元素的秩
    virtual Rank insert(Rank r, const T& e);
    
    // 删除元素，返回被删除的元素
    virtual T remove(Rank r);

    // 查找元素，返回查找到的元素的秩，未找到返回-1
    virtual Rank find(const T& e) const;

    template <typename Container>
    requires (clazy_framework::is_linear_structure<T, Container>)
    auto& operator=(const Container& L) {
        resize(L.size());
        copy(L.begin(), L.end(), begin());
        return *this;
    } // 线性表类型转换
};

// 以下是上述接口的实现部分
template <typename T, typename Allocator>
clazy_framework::AbstractAllocator* Vector<T, Allocator>::allocator = nullptr;

// 构造函数
template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(): Vector(default_min_capacity) {}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(int capacity) {
    _capacity = max(capacity, default_min_capacity);
    _data = new T[_capacity];
    _size = 0;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T, Allocator>& V): Vector(V._capacity) {
    _size = V._size;
    copy(V.begin(), V.begin() + _size, begin());
}

// 重新设置容量，这里总是重新开辟一块空间，并复制进去
template <typename T, typename Allocator>
void Vector<T, Allocator>::setCapacity(int capacity) {
    _capacity = max(capacity, default_min_capacity); // 重新设置capacity
    auto temp = new T[_capacity];                   // 申请一块新的空间
    _size = min(_size, _capacity);                  // size永远不能超过capacity，多余的元素弃掉
    copy(begin(), begin() + _size, temp);       // 复制数据到新的空间里
    delete[] _data;                             // 删除原有的数据区
    _data = temp;                               // 指向新的数据区
}

// 重新设置规模，这里要判断是否需要扩容或缩容
template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(int size) {
    if (allocator == nullptr) {
        allocator = new Allocator();
    }
    auto [action, capacity] = allocator->apply(_capacity, size);
    if (action != clazy_framework::AbstractAllocator::Result::NoAction) {
        setCapacity(capacity);              // 如果有必要，则进行扩容或者缩容
    }
    _size = size;                           // 重新设置规模
}

// 插入元素
template <typename T, typename Allocator>
Rank Vector<T, Allocator>::insert(Rank r, const T& e) {
    resize(_size + 1);                      // 向量的规模增加1
    copy_backward(begin() + r, begin() + _size - 1, begin() + _size); // 把后面的元素后移，思考一下，为什么要用backward？
    _data[r] = e;                           // 要插入的元素加入到向量中
    return r;
}

// 删除元素
template <typename T, typename Allocator>
T Vector<T, Allocator>::remove(Rank r) {
    T temp = _data[r];                      // 暂存被删除的元素
    copy(begin() + r + 1, begin() + _size, begin() + r); // 把后面的元素前移
    resize(_size - 1);                      // 向量的规模减少1
    return temp;                            // 返回被删除的元素
}

// 无序向量查找
template <typename T, typename Allocator>
Rank Vector<T, Allocator>::find(const T& e) const {
    for (Rank r : views::iota(0, size())) {       // 检测每个元素是否等于e
        if (_data[r] == e) {
            return r;                        // 如果找到就返回对应的秩
        }
    }
    return -1;                               // 如果没找到就返回-1
}

// 利用<<输出
template <typename T, typename Allocator>
ostream& operator<< (ostream& out, const Vector<T, Allocator>& V) {
    out << "[";
    for (Rank r : views::iota(0, V.size())) {
        if (r > 0) {
            out << ", ";
        }
        out << V[r];
    }
    return out << "]";
}

}
