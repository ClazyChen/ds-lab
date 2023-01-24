export module Stack.AbstractStack;

import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractStack : public DataStructure<T> {
public:
    virtual void push(const T& e) = 0; // ��ջ
    virtual void push(T&& e) = 0; // ��ջ
    virtual T pop() = 0; // ��ջ
    virtual T& top() = 0; // ȡջ��Ԫ��
    virtual const T& top() const {
        return const_cast<AbstractStack*>(this)->top();
    }
};

}
