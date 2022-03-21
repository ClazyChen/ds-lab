#pragma once

#include "list_dynamic.h"
#include "list_static.h"

// 包含此文件以包含动态链表和静态链表

namespace clazy {

// 最常见的链表赋予别名
template <typename T>
using ForwardList = DynamicList<T, ForwardListNode<T>>;

template <typename T>
using List = DynamicList<T, ListNode<T>>;

}