#pragma once

#include "list_framework.h"

namespace clazy {

template <typename T, typename P, typename Node>
using ListIterator = clazy_framework::AbstractList<T, P, Node>::Iterator;

// 列表的基本实现，这个实现是关于具体的位置无关的
// 在通常的列表中，位置使用指针即ListNodePos<T>实现
// 而在静态的列表中，位置使用偏移量Rank实现
// 第三个参数指示，是否是双向链表
template <typename T, typename P, typename Node>
requires (clazy_framework::is_listnode_type<T, Node>)
class BasicList : public clazy_framework::AbstractList<T, P, Node> {

// 在基本的列表实现中，仍然不对下面这些接口作出定义
// 留到具体的动态和静态列表的时候才作出定义
public:
    virtual T& data(P pos) const = 0;
    virtual P pred(P pos) const = 0;
    virtual P succ(P pos) const = 0;
    virtual P setPred(P pos, P pred) = 0;
    virtual P setSucc(P pos, P pred) = 0;
    virtual P invalid() const = 0;

// 为效率起见，我没有在执行各种操作时判断位置是否有效
// 如果需求鲁棒性，则可以进行这样的判断
// 但在实现setPred和setSucc时，需要判断位置是否有效

protected:
    P _head;   // 列表的首哨兵
    P _tail;   // 列表的尾哨兵
    int _size; // 列表的规模
    
    // 和内存管理有关的接口，在静态链表中，这些接口的实现是基于Container的
    // 而动态链表中，这些接口是通过new/delete实现的
    // 静态链表和动态链表的本质区别，是对申请和释放内存的区别
    // 申请新的节点的接口
    virtual P create() = 0;
    virtual P create(const T& e) = 0;
    // 释放一个节点的接口
    virtual void destroy(P pos) = 0;
    // 释放所有节点的接口，用于析构、清空这种需要整个列表全部删除的场合
    virtual void destroyAll() {}

    // 生成空表，直接连接头尾哨兵
    virtual void createEmptyList() {
        _size = 0;
        _head = create();
        _tail = create();
        setSucc(_head, _tail);
        setPred(_tail, _head);
    }
    // 复制列表，用于复制构造函数
    template <typename Container>
    requires (clazy_framework::is_linear_structure<T, Container>)
    void duplicateList(const Container& L);

public:
    BasicList() { } // 默认构造函数
    virtual int size() const { return _size; }
    virtual void clear() { destroyAll(); createEmptyList(); }
    virtual ListIterator<T, P, Node> begin() const { return ListIterator<T, P, Node>(succ(_head), this); }
    virtual ListIterator<T, P, Node> end() const { return ListIterator<T, P, Node>(_tail, this); }

    // 插入元素（包括前插和后插）的接口，返回被插入元素的位置
    virtual P insertAsPred(P pos, const T& e);
    virtual P insertAsSucc(P pos, const T& e);
    
    // 删除元素的接口，返回被删除的元素
    virtual T remove(P pos);

    // 查找元素的接口，返回查找到的元素的位置，未找到则返回无效位置
    virtual P find(const T& e) const;
};

// 复制构造函数，这里和正常的复制构造函数设计的不一样
// 因为我们希望允许动态和静态链表之间、双向和单向链表之间相互赋值
template <typename T, typename P, typename Node>
template <typename Container>
requires (clazy_framework::is_linear_structure<T, Container>)
void BasicList<T, P, Node>::duplicateList(const Container& C) {
    _size = C.size();
    _head = create();
    _tail = create();
    P last = _head, cur;
    for (auto it = C.begin(); it != C.end(); it++) { // 遍历L，建立连接
        cur = create(*it); // 复制一个节点
        last = setPred(cur, setSucc(last, cur)); // 将cur接在last后面
    }
    setPred(_tail, setSucc(last, _tail)); // 末尾连接上尾哨兵
}

// 插入的语义：
// 首哨兵的前插、尾哨兵的后插是未定义行为
// 其他都是允许的，并且返回被插入的位置

// 前插
// 对于单向链表，这里的方法会导致原先的位置pos在逻辑上变成了被前插的新元素的位置，从而产生不安全性
// 但另一方面，有的时候也能利用这个特性设计算法
template <typename T, typename P, typename Node>
P BasicList<T, P, Node>::insertAsPred(P pos, const T& e) {
    if constexpr (Node::isBidirectional()) {
        pos = insertAsSucc(pred(pos), e); // 等价于对前驱节点的后插
    } else /* forward list */ {       // 单向链表，用后插代替前插
        insertAsSucc(pos, data(pos)); // 注意这里进行后插的时候，可能会引入对尾哨兵的后插
        if (end().base() == pos) {    // 因此，有可能需要更新尾哨兵
            _tail = succ(pos);        // 为新插入的、新的尾哨兵
        }
        data(pos) = e;                // 然后对原来位置上的数据置为新数据
    }
    return pos;                       // 返回被插入的位置
}

// 后插
template <typename T, typename P, typename Node>
P BasicList<T, P, Node>::insertAsSucc(P pos, const T& e) {
    _size++;                          // 前插必定被转换为后插，所以只需要在后插这里更新规模
    P cur = create(e), next = succ(pos); // 创建新的节点cur
    setPred(cur, setSucc(pos, cur));  // 将cur接在被后插的节点pos后面
    setSucc(cur, setPred(next, cur)); // 将cur接在被前插的节点next前面
    return cur;                       // 返回创建的新节点
}

// 删除元素的语义：
// 永远不允许删除头哨兵或尾哨兵

// 删除元素
template <typename T, typename P, typename Node>
T BasicList<T, P, Node>::remove(P pos) {
    _size--;                          // 更新列表规模
    T temp = data(pos);               // 获取被删除的元素
    if constexpr (Node::isBidirectional()) { // 双向链表直接删除
        setSucc(pred(pos), succ(pos));
        setPred(succ(pos), pred(pos));
        destroy(pos);
    } else /* forward list */ {       // 单向链表采用和前插相似的技术
        P vic = succ(pos);            // 删除后继节点，并把pos更新为后继节点上的值
        data(pos) = data(vic);
        setSucc(pos, succ(vic));      // 将后继节点vic从链上摘下
        if (vic == _tail) {           // 有可能会因此摘除了尾哨兵
            _tail = pos;              // 需要重新定位尾哨兵的位置
        }
        destroy(vic);                 // 释放vic的空间
    }
    return temp;                      // 返回被删除的元素
}

// 查找元素的语义：
// 如果找到了目标元素，则返回一个位置
// 如果找不到目标元素，则返回无效位置
template <typename T, typename P, typename Node>
P BasicList<T, P, Node>::find(const T& e) const {
    for (auto it = begin(); it != end(); it++) {
        if (*it == e) {
            return it.base();
        }
    }
    return invalid();
}

// 利用<<输出
template <typename T, typename P, typename Node>
ostream& operator<< (ostream& out, const BasicList<T, P, Node>& L) {
    out << "L(";
    for (auto it = begin(L); it != end(L); it++) {
        if (it != begin(L)) {
            out << ", ";
        }
        out << *it;
    }
    return out << ")";
}

}
