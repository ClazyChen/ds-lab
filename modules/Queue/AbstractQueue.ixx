export module Queue.AbstractQueue;

import Framework.DataStructure;

export namespace dslab {

template <typename T>
class AbstractQueue : public DataStructure<T> {
public:
    virtual void enqueue(const T& e) = 0; // ���
    virtual void enqueue(T&& e) = 0; // ���
    virtual T dequeue() = 0; // ����
    virtual T& front() = 0; // ȡ����Ԫ��
    virtual const T& front() const {
        return const_cast<AbstractQueue*>(this)->front();
    }
};

}
