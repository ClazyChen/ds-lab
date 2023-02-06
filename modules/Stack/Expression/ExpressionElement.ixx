module;
#include <iostream>
#include <variant>
#include <unordered_map>

export module Expression.ExpressionElement;

import Factorial;
import Power;
import Expression.AbstractExpressionElement;

export namespace dslab {

class ExpressionElement : public AbstractExpressionElement {
    std::variant<char, int> m_element;
public:
    ExpressionElement() = default;
    ExpressionElement(char op) : m_element(op) {}
    ExpressionElement(int num) : m_element(num) {}

    char getOperator() const override {
        return std::get<char>(m_element);
    }

    const int& getOperand() const override {
        return std::get<int>(m_element);
    }

    bool isOperator() const override {
        return std::holds_alternative<char>(m_element);
    }

    bool operator==(char op) const override {
        return isOperator() && getOperator() == op;
    }

    bool operator!=(char op) const override {
        return isOperator() && getOperator() != op;
    }

    bool isOperand() const override {
        return std::holds_alternative<int>(m_element);
    }

    void setOperator(char op) override {
        m_element = op;
    }

    void setOperand(const int& num) override {
        m_element = num;
    }

    std::pair<bool, bool> operandPosition() const override {
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

    int apply(int lhs, int rhs) const override {
        if (isOperator()) {
            switch (getOperator()) {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '*': return lhs * rhs;
            case '/': return lhs / rhs;
            case '%': return lhs % rhs;
            case '^': return Power {}(lhs, rhs);
            case '!': return Factorial {}(lhs);
            }
        }
        return getOperand();
    }

    inline static const std::unordered_map<char, int> priority_left {
        {'(', 0}, {')', 0}, {'^', 3}, {'!', 4},
        {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
    };
    inline static const std::unordered_map<char, int> priority_right {
        {'(', 6}, {')', 0}, {'^', 3}, {'!', 4},
        {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
    };
    bool prior(char next) const override {
        if (isOperator()) {
            auto prior_left { priority_left.find(getOperator()) };
            auto prior_right { priority_right.find(next) };
            if (prior_left != priority_left.end() && prior_right != priority_right.end()) {
                return prior_left->second >= prior_right->second;
            }
        }
        return false;
    }

    bool operator==(const ExpressionElement& rhs) const {
        if (isOperator() && rhs.isOperator()) {
            return getOperator() == rhs.getOperator();
        } else if (isOperand() && rhs.isOperand()) {
            return getOperand() == rhs.getOperand();
        }
        return false;
    }

    bool operator!=(const ExpressionElement& rhs) const {
        return !(*this == rhs);
    }
};

std::ostream& operator<<(std::ostream& os, const ExpressionElement& e) {
    if (e.isOperator()) {
        os << e.getOperator();
    } else {
        os << e.getOperand();
    }
    return os;
}

}