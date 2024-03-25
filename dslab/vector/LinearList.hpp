#pragma once

#include "../framework.hpp"

namespace dslab::vector {

    template <typename T, typename It, typename CIt>
    class LinearList : public DataStructure<T> {
    public:
        // type traits
        using value_type = T;
        using iterator = It;
        using const_iterator = CIt;

        // insert element e at position p, and return the position of the new element
        virtual iterator insert(iterator p, const T& e) = 0;
        virtual iterator insert(iterator p, T&& e) = 0;

        // remove the element at position p, and return the position of the removed element's successor
        virtual iterator erase(iterator p) = 0;

        // try to find the element e, and return the position of the first occurrence
        virtual iterator find(const T& e) const = 0;

        // clear the list
        virtual void clear() = 0;

        // the first and last element's position
        virtual iterator begin() = 0;
        virtual iterator end() = 0;
        virtual const_iterator cbegin() const = 0;
        virtual const_iterator cend() const = 0;

        // get front and back
        T& front() { return *begin(); }
        T& back() { return *--end(); }

        // push front and back
        void push_front(const T& e) { insert(begin(), e); }
        void push_back(const T& e) { insert(end(), e); }
        void push_front(T&& e) { insert(begin(), std::move(e)); }
        void push_back(T&& e) { insert(end(), std::move(e)); }
        
        // pop front and back
        void pop_front() { erase(begin()); }
        void pop_back() { erase(--end()); }

    };

}