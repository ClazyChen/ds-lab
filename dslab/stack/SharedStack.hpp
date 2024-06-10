#pragma once

#include "AbstractStack.hpp"
#include "../vector.hpp"

namespace dslab::stack {

template <typename T, template<typename> typename L = DefaultVector, typename A = L<T>::allocator_type>
requires std::is_base_of_v<AbstractVector<T>, L<T>> && std::is_base_of_v<VectorAllocator, A>
class SharedStack : public DataStructure<T> {
protected:
    L<T> V {};
    A m_allocator {};
    std::size_t m_topb { static_cast<std::size_t>(-1) }, m_topf { 0 };
    
    bool full() const {
        return V.size() == 0 || m_topb + 1 == m_topf;
    }
    void expand() {
        std::size_t oldsize { V.size() };
        V.resize(m_allocator(V.capacity(), V.size()));
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
        const T& top() const {
            return S.V[S.m_topb + 1];
        }
        T pop() override {
            return std::move(S.V[++S.m_topb]);
        }
        std::size_t size() const override {
            return S.V.size() - 1 - S.m_topb;
        }
        void clear() override {
            S.m_topb = S.V.size() - 1;
        }
        std::string type_name() const override {
            return std::format("SharedStack<{}>::BackwardStack", S.V.type_name());
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
        const T& top() const {
            return S.V[S.m_topf - 1];
        }
        T pop() override {
            return std::move(S.V[--S.m_topf]);
        }
        std::size_t size() const override {
            return S.m_topf;
        }
        void clear() override {
            S.m_topf = 0;
        }
        std::string type_name() const override {
            return std::format("SharedStack<{}>::ForwardStack", S.V.type_name());
        }
    };

    BackwardStack Sb { *this };
    ForwardStack Sf { *this };
public:
    using value_type = T;

    SharedStack() = default;
    SharedStack(const SharedStack& other) : V(other.V), m_topb(other.m_topb), m_topf(other.m_topf) {}
    SharedStack(SharedStack&& other) noexcept : V(std::move(other.V)), m_topb(other.m_topb), m_topf(other.m_topf) {}
    SharedStack& operator=(const SharedStack& other) {
        V = other.V;
        m_topb = other.m_topb;
        m_topf = other.m_topf;
        return *this;
    }
    SharedStack& operator=(SharedStack&& other) noexcept {
        V = std::move(other.V);
        m_topb = other.m_topb;
        m_topf = other.m_topf;
        return *this;
    }
    virtual ~SharedStack() = default;

    SharedStack(std::initializer_list<std::initializer_list<T>> ilist) : SharedStack() {
        if (ilist.size() != 2) {
            throw std::invalid_argument("SharedStack initializer list must have exactly two elements");
        }
        auto& ilist0 { ilist.begin()[0] };
        auto& ilist1 { ilist.begin()[1] };
        V.resize(ilist0.size() + ilist1.size());
        m_topb = ilist0.size() - 1;
        m_topf = ilist0.size();
        std::move(ilist0.begin(), ilist0.end(), V.begin());
        std::move(ilist1.begin(), ilist1.end(), V.rbegin());
    }

    SharedStack& operator=(std::initializer_list<std::initializer_list<T>> ilist) {
        SharedStack tmp { ilist };
        *this = std::move(tmp);
        return *this;
    }

    std::pair<BackwardStack&, ForwardStack&> getStacks() {
        return { Sb, Sf };
    }

    std::pair<const BackwardStack&, const ForwardStack&> getStacks() const {
        return { Sb, Sf };
    }

    std::size_t size() const override {
        return Sb.size() + Sf.size();
    }

    std::string type_name() const override {
        return std::format("SharedStack<{}>", V.type_name());
    }

};

}