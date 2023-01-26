module;
#include <utility>

export module LinearList.AbstractLinearList;

import Framework.DataStructure;

export namespace dslab {

// �������Ա�
// T��ʾԪ�����ͣ�Pos��ʾԪ�ص�λ������
template <typename T, typename Pos>
class AbstractLinearList : public DataStructure<T> {
public:
    virtual T& get(Pos p) = 0; // ȡ��λ��p����Ԫ��
    virtual void set(Pos p, const T& e) = 0; // ��λ��p����Ԫ������Ϊe
    virtual void set(Pos p, T&& e) = 0; // ��λ��p����Ԫ������Ϊe

    virtual Pos insert(Pos p, const T& e) = 0; // ��λ��p������Ԫ��e
    virtual Pos insert(Pos p, T&& e) = 0; // ��λ��p������Ԫ��e
    virtual Pos find(const T& e) const = 0; // ����Ԫ��e
    virtual T remove(Pos p) = 0; // ɾ��λ��p����Ԫ��
    virtual void clear() = 0; // ������Ա�

    virtual Pos first() const = 0; // �������Ա�ĵ�һ��Ԫ�ص�λ��
    virtual Pos last() const = 0; // �������Ա�����һ��Ԫ�ص�λ��
    virtual Pos next(Pos p) const = 0; // ����λ��p�ĺ��λ��
    virtual Pos prev(Pos p) const = 0; // ����λ��p��ǰ��λ��
    virtual bool end(Pos p) const = 0; // �ж�λ��p�Ƿ��Ǳ�β

    virtual T& operator[](Pos p) {
        return get(p);
    }
    virtual T& front() {
        return get(first());
    }
    virtual T& back() {
        return get(last());
    }

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using position_type = Pos;

    virtual const T& get(Pos p) const {
        return const_cast<AbstractLinearList*>(this)->get(p);
    }
    virtual const T& operator[](Pos p) const {
        return get(p);
    }
    virtual const T& front() const {
        return get(first());
    }
    virtual const T& back() const {
        return get(last());
    }

    virtual void push_back(const T& e) {
        insert(next(last()), e);
    }
    virtual void push_back(T&& e) {
        insert(next(last()), std::move(e));
    }
    virtual void push_front(const T& e) {
        insert(first(), e);
    }
    virtual void push_front(T&& e) {
        insert(first(), std::move(e));
    }
    virtual T pop_back() {
        return remove(last());
    }
    virtual T pop_front() {
        return remove(first());
    }
};

}