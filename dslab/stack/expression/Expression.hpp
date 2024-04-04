#pragma once

#include "ExpressionElement.hpp"
#include "../../vector.hpp"
#include "../../stack.hpp"

namespace dslab::stack::expression {

template <typename T = int, template<typename> typename L = DefaultVector>
class Expression : public L<ExpressionElement<T>> {
    using L<ExpressionElement<T>>::push_back;
    using L<ExpressionElement<T>>::back;
public:
    Expression() = default;
    Expression(std::string_view expr, std::size_t base = 10) {
        auto start { 0uz };
        T temp {};
        auto add { [&](std::size_t end) {
            if (end > start) {
                std::from_chars(expr.data() + start, expr.data() + end, temp, base);
                push_back(std::move(temp));
            }
        } };
        for (auto i { 0uz }; i < expr.size(); i++) {
            if (!std::isdigit(c)) {
                add(i);
                if (c == '-' && (empty() || back() == '(')) {
                    start = i;
                } else {
                    start = i + 1;
                    push_back(c);
                }
            }
        }
        add(expr.size());
    }

    void infix2suffix() {
        Stack<ExpressionElement<T>> S;
        Expression<T> suffix;
        auto process { [&](const ExpressionElement<T>& e) {
            while (!S.empty() && S.top().prior(e.getOperator())) {
                if (auto op { S.pop() }; op != '(') {
                    suffix.push_back(std::move(op));
                } else {
                    break;
                }
            }
        } };
        for (auto& e : *this) {
            if (e.isOperand()) {
                suffix.push_back(std::move(e));
            } else {
                process(e);
                if (e != ')') {
                    S.push(std::move(e));
                }
            }
        }
        process(')');
        *this = std::move(suffix);
    }

    T calSuffix() const {
        Stack<T> S;
        for (auto& e : *this) {
            if (e.isOperand()) {
                S.push(e.getOperand());
            } else {
                auto [l, r] { e.operandPosition() };
                T rhs { r ? S.pop() : 0 };
                T lhs { l ? S.pop() : 0 };
                S.push(e.apply(lhs, rhs));
            }
        }
        return S.pop();
    }
};

}