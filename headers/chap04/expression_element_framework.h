#pragma once

#include "framework.h"

// 这个文件讨论的是表达式中的一个元素
// 它可以是运算符（operator）或者操作数（operand）
// 您在设计表达式计算的相关算法的时候，可以
// 1. 用我提供的这一套框架，这样只需要考虑表达式计算这个问题本身
// 2. 用我提供的抽象基类，但运算符类和操作数类自己写，此时您需要处理运算符优先级问题
// 3. 不用我提供的抽象基类，自己设计一套表达式计算的相关算法，这是一个有一定难度的工作

// 我在设计这个基类的时候，基于下面的基本性质：
//    在表达式中，永远不可能出现连续的两个操作数
// 所以，表达式中的元素可以被这样统一起来：
//    每个元素包括：一个运算符和紧随其后的一个操作数（如果有的话）
//    我给出每个运算符“前、后是否有操作数”这样的一个接口需要实现
// 这样的规定唯一会遇到困难的地方是表达式开头有可能是一个操作数
// 那么只需要在表达式的开头结尾强制加括号，就可以解决这个问题

// 这个类非常适合于中缀表达式的计算
// 但是对于后缀表达式，则没有这样优秀的性质
// 后缀表达式而言，每个元素只可能是操作数或者运算符，不能像这样进行压缩了

// 您如果设计自己的框架，可以抛开这个思路
// 比如，您可以把运算符和操作数分开，每一个元素存储一个union(char,int)
// 或者干脆放在2个栈里，实现方法有很多，邓俊辉《数据结构》里也有

namespace clazy_framework {

// 表达式中的元素的抽象基类
class AbstractExpressionElement : public Algorithm<int, int, int> {
public:
    virtual char getOperator() const = 0;
    virtual int getOperand() const = 0;
    virtual void setOperator(char operator_) = 0;
    virtual void setOperand(int operand) = 0;

    // 对于运算符来说，操作数的位置
    // 1. 对于前置运算符（如负号），返回(0, 1)
    // 2. 对于后置运算符（如阶乘），返回(1, 0)
    // 3. 对于中置运算符（如乘号），返回(1, 1)
    virtual pair<bool, bool> operandPosition() const = 0;

    // 运算符的计算结果
    // 1. 对于前置运算符，left无效，如负号返回(-right)
    // 2. 对于后置运算符，right无效，如阶乘返回(left!)
    // 3. 对于中置运算符，返回(left operator right)
    virtual int apply(int left, int right) = 0;

    // 运算符的优先级比较结果，是否优先级高于next
    virtual bool prior(const AbstractExpressionElement& next) const = 0;

    // 判断元素中的运算符和操作数是否有效
    virtual bool hasOperator() const = 0;
    virtual bool hasOperand() const = 0;

    // 重载运算符，这里也重载直接和字符进行比较
    bool operator==(const AbstractExpressionElement& other) const {
        return this->getOperator() == other.getOperator()
                && this->getOperand() == other.getOperand();
    }

    bool operator!=(const AbstractExpressionElement& other) const {
        return !(*this == other);
    }

    bool operator==(char c) const {
        return this->getOperator() == c;
    }

    bool operator!=(char c) const {
        return !(*this == c);
    }
};

}
