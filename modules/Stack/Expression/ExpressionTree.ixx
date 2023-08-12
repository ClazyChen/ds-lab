export module Expression.ExpressionTree;

import Expression.ExpressionElement;
import Tree;
import std;

export namespace dslab {

class ExpressionTree : public Tree<ExpressionElement> {
    int cal(TreeNodeConstPos<ExpressionElement> node) {
        int left { node->children().size() > 0 ? cal(node->child(0)) : 0 };
        int right { node->children().size() > 1 ? cal(node->child(1)) : 0 };
        return node->data().apply(left, right);
    }

public:
    ExpressionTree() = default;
    ExpressionTree(TreeNodeInst<ExpressionElement>&& root) : Tree<ExpressionElement>(std::move(root)) {}

    int cal() {
        return cal(this->root());
    }
};

}