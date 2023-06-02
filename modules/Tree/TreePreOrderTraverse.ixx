export module Tree.Traverse.TreePreOrderTraverse;

import Tree.TreeNode;
import Tree.Traverse.AbstractTreeTraverse;
import Stack;
import std;

export namespace dslab {

template <typename T>
class TreePreOrderTraverse : public AbstractTreeTraverse<T> {
public:
    void traverse(TreeNodeConstPos<T> p, std::function<void(TreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        visit(p);
        for (auto& child : p->children()) {
            traverse(child, visit);
        }
    }
    std::string type_name() const override {
        return "PreOrder Traverse";
    }
};

template <typename T>
class TreePreOrderTraverseIterative : public AbstractTreeTraverse<T> {
public:
    void traverse(TreeNodeConstPos<T> p, std::function<void(TreeNodeConstPos<T>)> visit) override {
        if (!p) {
            return;
        }
        Stack<TreeNodeConstPos<T>> S { p };
        while (!S.empty()) {
            auto node { S.pop() };
            visit(node);
            for (size_t i { node->children().size() }; i > 0; --i) {
                S.push(node->children()[i - 1]);
            }
        }
    }
    std::string type_name() const override {
        return "PreOrder Traverse (Iterative)";
    }
};

}