#pragma once

#include "binary_tree.h"
#include "expression_suffix.h"

// 这个文件实现了表达式树

namespace clazy {

using clazy_framework::BinaryTreeNode;

class ExpressionTree : public clazy_framework::AbstractExpression {
private:
    BinaryTree<ExpressionElement> BT;

    // 后缀表达式转表达式树
    void suffix2tree(const SuffixExpression& suffix);
public:
    void parse(string expr) override {
        SuffixExpression suffix;
        suffix.parse(expr);
        suffix2tree(suffix);
    }
    int getResult() const override;

    void clear() override {
        BT.clear();
    }
};

// 中缀表达式转换为表达式树
// 这个过程实际上可以包含计算，但为了表现表达式树的计算流程，我没有直接这么做
void ExpressionTree::suffix2tree(const SuffixExpression& suffix) {
    Stack<BinaryTreeNodePos<ExpressionElement>> S;
    for (auto& e : suffix.V) {
        auto node = new BinaryTreeNode<ExpressionElement>(e);
        if (e.hasOperand()) {
            S.push(node);
        } else {
            auto [l, r] = e.operandPosition();
            if (r) { node->setRight(S.pop()); }
            if (l) { node->setLeft(S.pop()); }
            S.push(node);
        }
    }
    BT.setRoot(S.pop());
}

// 表达式树计算（后序遍历）
// 这里计算过一遍之后就可以直接短路返回
int ExpressionTree::getResult() const {
    if (BT.empty()) { return 0; }
    if (BT.root()->data().hasOperand()) {
        return BT.root()->data().getOperand();
    }
    function<int(BinaryTreeNodePos<ExpressionElement>)> cal;
    cal = [&](auto pos) {
        if (pos == nullptr) {
            return 0;
        }
        auto& e = pos->data();
        if (!e.hasOperand()) {
            auto lv = cal(pos->left());
            auto rv = cal(pos->right());
            auto v  = e(lv, rv);
            e.setOperand(v);
        }
        return e.getOperand();
    };
    return cal(BT.root());
}


}