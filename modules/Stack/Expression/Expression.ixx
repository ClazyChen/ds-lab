module;
#include <string>

export module Expression;

export import Expression.AbstractExpressionElement;
export import Expression.ExpressionElement;

import Vector;
import Stack;

template <typename T>
using DefaultVector = dslab::Vector<T>;

// NOTE: 本文件在VS更新到17.5之后不能通过编译。
// 1 > D:\Workspace\ds - lab\modules\Stack\Expression\Expression.ixx(106, 1) : fatal  error C1001 : 内部编译器错误。
// 1 > (编译器文件“D:\a\_work\1\s\src\vctools\Compiler\CxxFE\sl\p1\c\module\writer.cpp”，第 1277 行)
// 等待 Microsoft Visual Studio 团队修复

export namespace dslab {

template <template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<ExpressionElement>, Vec<ExpressionElement>>
class Expression : public Vec<ExpressionElement> {
public:
    Expression() = default;
    Expression(const std::string& expr) {
        std::string expr2 { '(' + expr + ')' };
        std::string num;
        for (auto& c : expr2) {
            if (isdigit(c)) {
                num += c;
            } else {
                if (!num.empty()) {
                    this->push_back(ExpressionElement(std::stoi(num)));
                    num.clear();
                }
                if (c == '-' && this->back() == '(') {
                    num += c;
                } else {
                    this->push_back(ExpressionElement(c));
                }
            }
        }
    }

    /*void infix2suffix() {
        Stack<ExpressionElement> S;
        Expression suffix;
        for (auto& e : *this) {
            if (e.isOperand()) {
                suffix.push_back(std::move(e));
            } else {
                while (!S.empty() && S.top().prior(e.getOperator())) {
                    if (auto op { S.pop() }; op != '(') {
                        suffix.push_back(std::move(op));
                    } else {
                        break;
                    }
                }
                if (e != ')') {
                    S.push(std::move(e));
                }
            }
        }
        *this = std::move(suffix);
    }

    int calSuffix() const {
        Stack<int> S;
        for (auto& e : *this) {
            if (e.isOperand()) {
                S.push(e.getOperand());
            } else {
                auto [l, r] { e.operandPosition() };
                int rhs { r ? S.pop() : 0 };
                int lhs { l ? S.pop() : 0 };
                S.push(e.apply(lhs, rhs));
            }
        }
        return S.pop();
    }

    int calInfix() const {
        Stack<int> Sr;
        Stack<ExpressionElement> So;
        for (auto& e : *this) {
            if (e.isOperand()) {
                Sr.push(e.getOperand());
            } else {
                while (!So.empty() && So.top().prior(e.getOperator())) {
                    if (auto op { So.pop() }; op != '(') {
                        auto [l, r] { op.operandPosition() };
                        int rhs { r ? Sr.pop() : 0 };
                        int lhs { l ? Sr.pop() : 0 };
                        Sr.push(op.apply(lhs, rhs));
                    } else {
                        break;
                    }
                }
                if (e != ')') {
                    So.push(e);
                }
            }
        }
        return Sr.pop();
    }*/
};

}