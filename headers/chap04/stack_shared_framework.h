#pragma once

#include "stack_framework.h"

// 这一节给出了共享栈的一个框架
// 共享栈指的是，两个栈共享一个数组
// 数组的左侧、右侧分别对应两个栈的底部，数据向中间延伸
// 共享栈的操作包括：
// 1. 判满、获取元素数量（总和）、清空
//    判空操作仍可以继承size() == 0的判断方式
// 2. 获取两个栈的引用

namespace clazy_framework {

template <typename T>
class AbstractSharedStack : public DataStructure<T> {
public:
    virtual bool full() const = 0;
    virtual int size() const = 0;
    virtual void clear() = 0;
    virtual pair<AbstractStack<T>&, AbstractStack<T>&> getStacks() = 0;
};

}