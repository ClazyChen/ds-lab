export module BinarySearchTree.AbstractBinarySearchTree;

import BinaryTree;
import Vector;
import std;

export namespace dslab {

template <typename T, typename Cmp = std::less<T>>
class AbstractBinarySearchTree : public BinaryTree<T> {
protected:
    Cmp cmp;

    BinaryTreeInOrderIterate<T> inOrderIterate;
    BinaryTreeInOrderReverseIterate<T> inOrderReverseIterate;

    BinaryTreeNodeConstPos<T> succ(BinaryTreeNodeConstPos<T> p) const {
        return inOrderIterate.next(p);
    }

    BinaryTreeNodeConstPos<T> pred(BinaryTreeNodeConstPos<T> p) const {
        return inOrderReverseIterate.next(p);
    }

public:
    using BinaryTree<T>::BinaryTree;
    virtual BinaryTreeNodePos<T> insert(const T& e) = 0;
    virtual BinaryTreeNodePos<T> insert(T&& e) = 0;
    virtual BinaryTreeNodeConstPos<T> search(const T& e) const = 0;
    virtual Vector<BinaryTreeNodeConstPos<T>> searchAll(const T& e) const = 0;
    virtual bool remove(const T& e) = 0;

    using iterator = BinaryTreeIterator<T, BinaryTreeInOrderIterate>;
    using const_iterator = BinaryTreeConstIterator<T, BinaryTreeInOrderIterate>;

    iterator begin() noexcept {
        return iterator::begin(this->root());
    }

    iterator end() noexcept {
        return iterator::end(this->root());
    }

    const_iterator begin() const noexcept {
        return const_iterator::begin(this->root());
    }

    const_iterator end() const noexcept {
        return const_iterator::end(this->root());
    }

    const_iterator cbegin() const noexcept {
        return const_iterator::begin(this->root());
    }

    const_iterator cend() const noexcept {
        return const_iterator::end(this->root());
    }

    using reverse_iterator = BinaryTreeIterator<T, BinaryTreeInOrderReverseIterate>;
    using const_reverse_iterator = BinaryTreeConstIterator<T, BinaryTreeInOrderReverseIterate>;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator::begin(this->root());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator::end(this->root());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator::begin(this->root());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator::end(this->root());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator::begin(this->root());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator::end(this->root());
    }
};

}