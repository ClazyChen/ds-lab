#pragma once

// 这个文件给出了一个线性表的框架
// 向量和列表共享这个框架

// 线性表的基本性质：
// 表中的元素具有逻辑上的顺序性，表中元素有其先后次序

// 所以，我们在讨论线性表的时候，采用下面的构造
// 接受两个模板参数，分别表示元素类型和位置类型
// 接口如下：
// 1. 从位置获取元素
// 2. 从位置获取前一个元素的位置
// 3. 从位置获取后一个元素的位置
// 4. 获取第一个元素的位置
// 5. 获取最后一个元素的位置
// 6. 获取一个无效的位置

#include "framework.h"

namespace clazy_framework {

// 线性表的ADT接口
template <typename T, typename P>
class AbstractLinearStructure : public DataStructure<T> {
public:
    using Position = P;

    virtual T& get(P pos) const = 0;
    virtual P getPred(P pos) const = 0;
    virtual P getSucc(P pos) const = 0;
    virtual P getFrontPos() const = 0;
    virtual P getBackPos() const = 0;
    virtual P invalidPos() const = 0;

    // 获取第一个、最后一个、任意位置的元素
    T& front() const {
        return get(getFrontPos());
    }
    T& back() const {
        return get(getBackPos());
    }
    T& operator[](P pos) const {
        return get(pos);
    }

    // 插入和删除第一个和最后一个元素
    virtual void push_back(const T& e) = 0;
    virtual void push_front(const T& e) = 0;
    virtual T pop_back() = 0;
    virtual T pop_front() = 0;

};

template <typename T, typename P, typename Container>
concept is_linear_structure = is_base_of_v<AbstractLinearStructure<T, P>, Container>;

}
