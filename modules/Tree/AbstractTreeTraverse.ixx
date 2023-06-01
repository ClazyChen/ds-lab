export module Tree.Traverse.AbstractTreeTraverse;

import Framework.Algorithm;
import Tree.TreeNode;
import std;

export namespace dslab {

template <typename T>
class AbstractTreeTraverse : public Algorithm<void(TreeNodeConstPos<T>, std::function<void(const T&)>)> {
public:
    virtual void traverse(TreeNodeConstPos<T> p, std::function<void(TreeNodeConstPos<T>)> visit) = 0;
    void operator()(TreeNodeConstPos<T> p, std::function<void(const T&)> visit) override {
        traverse(p, [&visit](TreeNodeConstPos<T> p) { visit(p->data()); });
    }
};

}