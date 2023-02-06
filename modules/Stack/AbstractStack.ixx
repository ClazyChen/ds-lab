export module Stack.AbstractStack;

import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractStack : public DataStructure<T> {
public:
    virtual void push(const T& e) = 0; // 入栈
    virtual void push(T&& e) = 0; // 入栈
    virtual T pop() = 0; // 出栈
    virtual T& top() = 0; // 取栈顶元素
    virtual const T& top() const {
        return const_cast<AbstractStack*>(this)->top();
    }
};

}
