export module Expression.AbstractExpressionElement;

import std;

export namespace dslab {

class AbstractExpressionElement {
public:
    virtual char getOperator() const = 0;
    virtual const int& getOperand() const = 0;
    virtual bool isOperator() const = 0;
    virtual bool isOperand() const = 0;
    virtual bool operator==(char op) const = 0;
    virtual bool operator!=(char op) const = 0;
    virtual void setOperator(char op) = 0;
    virtual void setOperand(const int& num) = 0;
    virtual std::pair<bool, bool> operandPosition() const = 0;
    virtual bool prior(char next) const = 0;
    virtual int apply(int lhs, int rhs) const = 0;
};

}