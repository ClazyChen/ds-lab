export module Tree.AbstractTree;

import Framework.DataStructure;
import Tree.TreeNode;
import std;

export namespace dslab {

template <typename T>
class AbstractTree : public DataStructure<T> {
public:
    virtual TreeNodePos<T> root() = 0;
    virtual TreeNodeConstPos<T> root() const = 0;
    virtual size_t size() const = 0;
    virtual TreeNodePos<T> insertAsChild(TreeNodePos<T> p, const T& e, size_t index) = 0;
    virtual TreeNodePos<T> insertAsChild(TreeNodePos<T> p, T&& e, size_t index) = 0;
    virtual TreeNodePos<T> insertAsRoot(const T& e) = 0;
    virtual TreeNodePos<T> insertAsRoot(T&& e) = 0;
    virtual TreeNodePos<T> find(const T& e) const = 0;
    virtual T remove(TreeNodePos<T> p) = 0;
    virtual TreeNodePos<T> attachAsChild(TreeNodePos<T> p, TreeNodeInst<T>&& st, size_t index) = 0;
    virtual TreeNodeInst<T> detach(TreeNodePos<T> p) = 0;
    virtual void clear() = 0;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractTree<T>& tree) {
    os << "Tree[";
    if (tree.root()) {
        os << *tree.root();
    }
    os << "]";
    return os;
}

}