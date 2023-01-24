module;
#include <algorithm>

export module Stack.SharedStack;

import Vector;
import Framework.DataStructure;
import Stack.AbstractStack;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class SharedStack : public DataStructure<T> {
    Vec<T> V {};
    Rank m_topb { -1 }, m_topf { 0 };
    bool full() const {
        return V.size() == 0 || m_topb + 1 == m_topf;
    }
    void expand() {
        size_t oldsize { V.size() };
        V.resize(V.capacity() + 1);
        V.resize(V.capacity());
        std::move_backward(V.begin() + m_topb + 1, V.begin() + oldsize, V.end());
        m_topb += V.size() - oldsize;
    }
    
    class BackwardStack : public AbstractStack<T> {
        SharedStack& S;
    public:
        BackwardStack(SharedStack& s) : S(s) {}
        void push(const T& e) override {
            if (S.full()) {
                S.expand();
            }
            S.V[S.m_topb--] = e;
        }
        void push(T&& e) override {
            if (S.full()) {
                S.expand();
            }
            S.V[S.m_topb--] = std::move(e);
        }
        T& top() override {
            return S.V[S.m_topb + 1];
        }
        T pop() override {
            return std::move(S.V[++S.m_topb]);
        }
        size_t size() const override {
            return S.V.size() - 1 - S.m_topb;
        }
    };

    class ForwardStack : public AbstractStack<T> {
        SharedStack& S;
    public:
        ForwardStack(SharedStack& s) : S(s) {}
        void push(const T& e) override {
            if (S.full()) {
                S.expand();
            }
            S.V[S.m_topf++] = e;
        }
        void push(T&& e) override {
            if (S.full()) {
                S.expand();
            }
            S.V[S.m_topf++] = std::move(e);
        }
        T& top() override {
            return S.V[S.m_topf - 1];
        }
        T pop() override {
            return std::move(S.V[--S.m_topf]);
        }
        size_t size() const override {
            return S.m_topf;
        }
    };

    BackwardStack Sb;
    ForwardStack Sf;
public:
    SharedStack() : Sb(*this), Sf(*this) {}
    SharedStack(const SharedStack& rhs) : V(rhs.V), m_topb(rhs.m_topb), m_topf(rhs.m_topf), Sb(*this), Sf(*this) {}
    SharedStack(SharedStack&& rhs) : V(std::move(rhs.V)), m_topb(rhs.m_topb), m_topf(rhs.m_topf), Sb(*this), Sf(*this) {}
    SharedStack& operator=(const SharedStack& rhs) {
        V = rhs.V;
        m_topb = rhs.m_topb;
        m_topf = rhs.m_topf;
        return *this;
    }
    SharedStack& operator=(SharedStack&& rhs) {
        V = std::move(rhs.V);
        m_topb = rhs.m_topb;
        m_topf = rhs.m_topf;
        return *this;
    }
    virtual ~SharedStack() = default;

    std::pair<AbstractStack<T>&, AbstractStack<T>&> getStacks() {
        return { Sb, Sf };
    }
};


}