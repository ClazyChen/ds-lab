export module Stack.SharedStack;

import Vector;
import Framework.DataStructure;
import Stack.AbstractStack;
import std;

export namespace dslab {

template <typename T, template<typename> typename Vec = DefaultVector>
    requires std::is_base_of_v<AbstractVector<T>, Vec<T>>
class SharedStack : public DataStructure<T> {
    Vec<T> V {};
    Rank m_topb { static_cast<Rank>(-1) }, m_topf {0};
    bool full() const {
        return V.size() == 0 || m_topb + 1 == m_topf;
    }
    void expand() {
        size_t oldsize { V.size() };
        V.resize(std::max(V.capacity() + 1, V.capacity() * 2));
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

    SharedStack(std::initializer_list<std::initializer_list<T>> ilist) : SharedStack() {
        if (ilist.size() != 2) {
            throw std::invalid_argument("SharedStack: initializer list must have 2 elements");
        }
        auto ilist0 { std::move(begin(ilist)[0]) };
        auto ilist1 { std::move(begin(ilist)[1]) };
        V.resize(ilist0.size() + ilist1.size());
        m_topb = ilist0.size() - 1;
        m_topf = ilist0.size();
        std::move(begin(ilist0), end(ilist0), begin(V));
        std::move(begin(ilist1), end(ilist1), rbegin(V));
    }

    SharedStack& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        SharedStack tmp { ilist };
        *this = std::move(tmp);
        return *this;
    }

    std::pair<AbstractStack<T>&, AbstractStack<T>&> getStacks() {
        return { Sb, Sf };
    }

    size_t size() const override {
        return Sb.size() + Sf.size();
    }

    template <typename T1, template<typename> typename V1>
    friend std::ostream& operator<<(std::ostream& os, const SharedStack<T1, V1>& S);
};

template <typename T, template<typename> typename Vec>
std::ostream& operator<<(std::ostream& os, const SharedStack<T, Vec>& S) {
    os << "STACK(Shared)[";
    for (size_t i { 0 }; i < S.m_topf; ++i) {
        os << S.V[i] << ", ";
    }
    os << "-> ";
    if (auto step = S.m_topb - S.m_topf + 1; step > 0) {
        os << std::format("...({})...", step);
    }
    os << " <-";
    for (size_t i { S.m_topb + 1 }; i < S.V.size(); ++i) {
        os << ", " << S.V[i];
    }
    os << "]";
    return os;
}

}