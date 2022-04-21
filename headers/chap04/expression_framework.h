#pragma once

#include "framework.h"

// 这个文件规定了表达式的基类
// 它接收一个表达式字符串，对它进行解析
// 然后计算表达式的值

namespace clazy_framework {

class AbstractExpression : public Algorithm<int, string> {
public:
    virtual void parse(string expr) = 0;
    virtual int getResult() const = 0;

    int apply(string expr) override {
        parse(expr);
        return getResult();
    }
};

}