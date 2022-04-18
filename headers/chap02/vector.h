#pragma once

// 这个文件定义了一个向量的实现
// 这是我给出的示例实现，它实现了抽象向量的基类中的纯虚函数

#include "vector_framework.h"
#include "allocator.h"

namespace clazy {

using Rank = clazy_framework::Rank;

using DefaultVectorAllocator = RatioAllocator<2, 1>;

// 向量
template <typename T, typename Alloc = DefaultVectorAllocator>
requires (is_base_of_v<clazy_framework::AbstractAllocator, Alloc>)
class Vector : public clazy_framework::AbstractVector<T> {
protected:
    T* _data;                // 向量的数据区
    int _capacity;           // 向量的容量
    int _size;               // 向量的规模
    Alloc allocator;         // 向量扩容缩容的策略

public:
    Vector();                // 默认构造函数，生成空向量
    Vector(int size);        // 生成指定规模的空向量
    Vector(const Vector& V); // 复制构造函数，复制向量
    Vector(Vector&& V);      // 移动构造函数，移动向量
    ~Vector();               // 析构函数，释放向量占用的空间
    auto& operator=(const Vector& V); // 赋值运算符，复制向量
    auto& operator=(Vector&& V);      // 赋值运算符，移动向量

    int capacity() const override { return _capacity; }
    int size() const override { return _size; }
    T* data() const override { return _data; }

    void reserve(int capacity) override;
    void resize(int size) override;
    Rank insert(Rank pos, const T& e) override;
    T remove(Rank pos) override;
    Rank find(const T& e) const override;
};

// 构造函数、析构函数和赋值运算符
template <typename T, typename Alloc>
Vector<T, Alloc>::Vector() {
    _data = nullptr;
    _capacity = 0;
    _size = 0;
}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(int size): Vector() {
    if (size > 0) {
        _data = new T[size];
        _capacity = size;
        _size = size;
    }
}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(const Vector& V): Vector(V.capacity()) {
    copy(V._data, V._data + V._size, _data);
    _size = V._size;
}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(Vector&& V): Vector() {
    _data = V._data;
    _capacity = V._capacity;
    _size = V._size;
    V._data = nullptr;
    V._capacity = 0;
    V._size = 0;
}

template <typename T, typename Alloc>
Vector<T, Alloc>::~Vector() {
    if (_data) {
        delete[] _data;
    }
}

template <typename T, typename Alloc>
auto& Vector<T, Alloc>::operator=(const Vector& V) {
    if (this != &V) {
        if (_capacity < V._size) {
            reserve(V._size);
        }
        copy(V._data, V._data + V._size, _data);
        _size = V._size;
    }
    return *this;
}

template <typename T, typename Alloc>
auto& Vector<T, Alloc>::operator=(Vector&& V) {
    if (this != &V) {
        _data = V._data;
        _capacity = V._capacity;
        _size = V._size;
        V._data = nullptr;
        V._capacity = 0;
        V._size = 0;
    }
    return *this;
}

// 重新设置容量，这里和普通向量不同，允许用户用这个函数缩小容量
template <typename T, typename Alloc>
void Vector<T, Alloc>::reserve(int capacity) {
    T* old_data = _data;
    _capacity = capacity;
    _size = min(_size, _capacity);
    if (_capacity > 0) {
        _data = new T[_capacity];
    } else {
        _data = nullptr;
    }
    copy(old_data, old_data + _size, _data);
    if (old_data) {
        delete[] old_data;
    }
}

// 重新设置规模，这里要判断是否需要扩容或缩容
template <typename T, typename Alloc>
void Vector<T, Alloc>::resize(int size) {
    if (size < 0) {
        throw runtime_error("size is negative");
    }
    auto [action, capacity] = allocator(_capacity, size);
    if (action != clazy_framework::AllocateResult::NoAction) {
        reserve(capacity);
    }
    _size = size;
}

// 插入元素
template <typename T, typename Alloc>
Rank Vector<T, Alloc>::insert(Rank pos, const T& e) {
    if (pos < 0 || pos > _size) {
        throw runtime_error("pos is invalid");
    }
    resize(_size + 1);
    copy_backward(_data + pos, _data + _size - 1, _data + _size);
    _data[pos] = e;
    return pos;
}

// 删除元素
template <typename T, typename Alloc>
T Vector<T, Alloc>::remove(Rank pos) {
    if (pos < 0 || pos >= _size) {
        throw runtime_error("pos is invalid");
    }
    T e = _data[pos];
    copy(_data + pos + 1, _data + _size, _data + pos);
    resize(_size - 1);
    return e;
}

// 查找元素，返回元素的位置，如果没有找到，返回-1
template <typename T, typename Alloc>
Rank Vector<T, Alloc>::find(const T& e) const {
    Rank pos = 0;
    while (pos < _size && _data[pos] != e) {
        ++pos;
    }
    return pos < _size ? pos : invalidPos();
}

// 利用<<输出
template <typename T, typename Alloc>
ostream& operator<< (ostream& out, const Vector<T, Alloc>& V) {
    out << "[";
    for (int i = 0; i < V.size(); i++) {
        out << V[i];
        if (i < V.size() - 1) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}

}