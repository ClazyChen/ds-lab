#pragma once

#include "../../framework.hpp"
#include <variant>

namespace dslab::stack::expression {
    template <typename T>
    class ExpressionElement {
        std::variant<char, T> m_element;
        Power<T> m_power;
        Factorial<T> m_factorial;
    public:
        ExpressionElement() = default;
        ExpressionElement(char op) : m_element(op) {}
        ExpressionElement(const T& num) : m_element(num) {}
        ExpressionElement(T&& num) : m_element(std::move(num)) {}

        char getOperator() const {
            return std::get<char>(m_element);
        }

        const T& getOperand() const {
            return std::get<T>(m_element);
        }

        bool isOperator() const {
            return std::holds_alternative<char>(m_element);
        }

        bool operator==(char op) const {
            return isOperator() && getOperator() == op;
        }

        bool operator!=(char op) const {
            return isOperator() && getOperator() != op;
        }

        bool isOperand() const {
            return std::holds_alternative<T>(m_element);
        }

        void setOperator(char op) {
            m_element = op;
        }

        void setOperand(const T& num) {
            m_element = num;
        }

        void setOperand(T&& num) {
            m_element = std::move(num);
        }

        std::pair<bool, bool> operandPosition() const {
            if (isOperator()) {
                switch (getOperator()) {
                case '(': return { false, true };
                case ')': case '!': return { true, false };
                case '+': case '-':
                case '*': case '/':
                case '%': case '^': return { true, true };
                }
            }
            return { false, false };
        }

        T apply(const T& lhs, const T& rhs) const {
            if (isOperator()) {
                switch (getOperator()) {
                case '+': return lhs + rhs;
                case '-': return lhs - rhs;
                case '*': return lhs * rhs;
                case '/': return lhs / rhs;
                case '%': return lhs % rhs;
                case '^': return m_power(lhs, rhs);
                case '!': return m_factorial(lhs);
                }
            }
            return getOperand();
        }

        bool operator==(const ExpressionElement& other) const {
            return m_element == other.m_element;
        }

        inline static const std::unordered_map<char, int> priority_left {
            {'(', 0}, {')', 0}, {'^', 3}, {'!', 4},
            {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
        };
        inline static const std::unordered_map<char, int> priority_right {
            {'(', 6}, {')', 0}, {'^', 3}, {'!', 4},
            {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
        };

        bool prior(char next) const {
            if (isOperator()) {
                auto prior_left { priority_left.find(getOperator()) };
                auto prior_right { priority_right.find(next) };
                if (prior_left != priority_left.end() && prior_right != priority_right.end()) {
                    return prior_left->second >= prior_right->second;
                }
            }
            return false;
        }

        friend std::ostream& operator<<(std::ostream& os, const ExpressionElement& e) {
            if (e.isOperator()) {
                os << e.getOperator();
            } else {
                os << e.getOperand();
            }
            return os;
        }
    };
}