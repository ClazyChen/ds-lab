export module BinaryTree.AbstractBinaryTree;

import BinaryTree.BinaryTreeNode;
import Framework.DataStructure;
import Framework.PointerProxy;
import Queue;
import std;

export namespace dslab {

template <typename T, template<typename> typename Node>
class AbstractTemplateBinaryTree : public DataStructure<T> {
    using NodeInst = UniquePointerProxy<Node<T>>;
    using NodePos = RawPointerProxy<Node<T>>;
    using NodeConstPos = ConstRawPointerProxy<Node<T>>;
public:
    virtual NodePos root() = 0;
    virtual NodeConstPos root() const = 0;
    virtual size_t size() const = 0;
    virtual NodePos insertAsLeftChild(NodePos p, const T& e) = 0;
    virtual NodePos insertAsRightChild(NodePos p, const T& e) = 0;
    virtual NodePos insertAsLeftChild(NodePos p, T&& e) = 0;
    virtual NodePos insertAsRightChild(NodePos p, T&& e) = 0;
    virtual NodePos insertAsRoot(const T& e) = 0;
    virtual NodePos insertAsRoot(T&& e) = 0;
    virtual NodePos attachAsLeftChild(NodePos p, NodeInst&& st) = 0;
    virtual NodePos attachAsRightChild(NodePos p, NodeInst&& st) = 0;
    virtual NodePos find(const T& e) const = 0;
    virtual T remove(NodePos p) = 0;
    virtual NodeInst detach(NodePos p) = 0;
    virtual void clear() = 0;
};

template <typename T>
using AbstractBinaryTree = AbstractTemplateBinaryTree<T, BinaryTreeNode>;

template <typename T, template<typename> typename Node>
std::ostream& operator<<(std::ostream& os, const AbstractTemplateBinaryTree<T, Node>& tree) {
    os << "BT[";
    if (tree.root()) {
        Queue<ConstRawPointerProxy<Node<T>>> Q { tree.root() };
        size_t count { 1 };
        while (count > 0) {
            if (auto f { Q.dequeue() }; f) {
                os << f->data() << ", ";
                --count;
                Q.enqueue(f->left());
                Q.enqueue(f->right());
                if (f->left()) {
                    ++count;
                }
                if (f->right()) {
                    ++count;
                }
            } else {
                os << "^, ";
            }
        }
        os << "\b\b";
    }
    os << "]";
    return os;
}

}