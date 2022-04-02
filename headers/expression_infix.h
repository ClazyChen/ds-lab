#pragma once
#include "expression_element.h"
#include "stack.h"

// 这个文件实现了中缀表达式

namespace clazy {

// 线性表达式的公共基类
class LinearExpression : public clazy_framework::AbstractExpression {
protected:
    Vector<clazy_framework::AbstractExpressionElement*> V; // 用来存储表达式的元素

    void removeElements() { // 删除所有的元素
        for (auto e : V) {
            delete e;
        }
        V.clear();
    }

public:
    virtual ~LinearExpression() { removeElements(); }
};

// 中缀表达式的类
class InfixExpression : public LinearExpression {
public:
    virtual int getResult() override;
    virtual void parse(string expr) override;

    InfixExpression() {}
    InfixExpression(string expr) { parse(expr); }

    friend ostream& operator<<(ostream& out, const InfixExpression& expr);
    friend class SuffixExpression; // 后缀表达式那里需要实现中缀转后缀的功能
};

// 字符串转换为中缀表达式
// 这里在表达式的开头和结尾强制加了一对括号，以减少计算中的特判
void InfixExpression::parse(string expr) {
    removeElements();               // 清除原有的元素
    V.push_back(new Operator('(')); // 在表达式的开头加上一个括号
    int numberBuffer = 0;           // 用来存放正在读取的数字
    bool isReadingNumber = false;
    for (char c : expr) {
        if (isdigit(c)) {
            numberBuffer = numberBuffer * 10 + c - '0';
            isReadingNumber = true;
        } else {
            if (isReadingNumber) {
                V.push_back(new Operand(numberBuffer));
                numberBuffer = 0;
                isReadingNumber = false;
            } else {
                if (c == '-' && (V.size() == 0 || V.back()->getOperator() == '(')) {
                    V.push_back(new Operator('@'));
                    continue;       // 特判负号
                }
            }
            V.push_back(new Operator(c));
        }
    }
    if (isReadingNumber) {          // 如果最后以操作数结尾，则需要将最后的数字加入
        V.push_back(new Operand(numberBuffer));
    }
    V.push_back(new Operator(')')); // 在表达式的结尾加上一个括号
}

// 中缀表达式计算方法的设计思路
// 这里用MixedElement来存放操作数和运算符，保证每个元素中都必定含有运算符，但不一定含有操作数
// 这个设计的基础是，在表达式中，任何一个操作数必定在某个运算符的后面
// 所以，MixedElement把一个运算符和它后方（如果有的话）的操作数放在一起
// 下面的算法是处理不了非法表达式的（比如，可能会发生对空栈还在取top的情况，从而造成访存越界）
// 这里用了智能指针，目的只是为了不需要手动释放内存

// 中缀表达式的计算
int InfixExpression::getResult() {
    Stack<shared_ptr<MixedElement>> S;            // 用来存放操作数和运算符的栈
    for (auto e : V) {                            // 遍历中缀表达式
        int operand = e->getOperand();            // 将要被插入的操作数
        if (e->isOperator()) {                    // 如果是运算符，分成两种情况，栈顶优先级更高或者栈顶优先级更低
            while (!S.empty() && S.top()->higherPriority(*e)) {  // 只要栈顶的优先级更高，就优先计算栈顶的运算符
                if (auto t = S.pop(); t->getOperator() == '(') { // 如果是括号，则将括号弹出，并把括号内的操作数取出来
                    operand = t->getOperand();    // 左括号仅仅比右括号的优先级高
                    if (S.empty()) { return operand; } // 因为外层加了一对括号，当且仅当空栈，意味着运算结束
                    break;
                } else {                          // 否则，执行运算，将运算结果压回栈
                    S.top()->setOperand(t->apply(S.top()->getOperand(), t->getOperand()));    
                }
            }
        }
        if (e->isOperand() || e->getOperator() == ')') { // e是操作数或e为右括号的情况
            S.top()->setOperand(operand);         // 将操作数压回栈顶
        } else {                                  // 其他情况，让新的运算符入栈
            S.push(make_shared<MixedElement>(e));
        }
    }
    return 0; // 不应该到达这里，合法表达式应该在前面就返回了
}

ostream& operator<<(ostream& out, const InfixExpression& expr) {
    for (auto e : expr.V) {
        out << *e;
    }
    return out;
}

}