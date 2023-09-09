export module PriorityQueue.LeftistHeap;

import PriorityQueue.AbstractPriorityQueue;
import BinaryTree;
import Vector;
import std;

export namespace dslab {

template <typename T>
class LeftistHeap : public AbstractPriorityQueue<T> {
    struct Element {
        T value;
        size_t npl { 1 };
        Element(T value) : value(value) {}
        operator T() const { return value; }
        bool operator==(const Element& e) const { return value == e.value; }
    };

    static size_t npl(BinaryTreeNodeConstPos<Element> node) {
        return node ? node->data().npl : 0;
    }
    
    BinaryTree<Element> H;

    BinaryTreeNodeInst<Element> merge(BinaryTreeNodeInst<Element>&& H1, BinaryTreeNodeInst<Element>&& H2) {
        if (!H1) return std::move(H2);
        if (!H2) return std::move(H1);
        if (this->cmp(H2->data(), H1->data())) {
            std::swap(H1, H2);
        }
        H1->right() = merge(std::move(H1->right()), std::move(H2));
        H1->right()->parent() = H1;
        if (npl(H1->left()) < npl(H1->right())) {
            std::swap(H1->left(), H1->right());
        }
        H1->data().npl = npl(H1->right()) + 1;
        return H1;
    }

    LeftistHeap(BinaryTree<Element>&& H) : H(std::move(H)) {}

public:
    LeftistHeap() : AbstractPriorityQueue<T>() {}

    LeftistHeap(Vector<T>&& V, const std::function<bool(const T&, const T&)>& cmp = std::less<T>()) : AbstractPriorityQueue<T>(cmp) {
        Vector<BinaryTreeNodeInst<Element>> nodes;
        for (auto&& e : V) {
            nodes.push_back(BinaryTreeNodeInst<Element>::make(std::move(e)));
        }
        if (auto n { nodes.size() }; n == 0) {
            return;
        } else if (n > 1) {
            for (auto r { (n - 2) / 2 }; ; --r) {
                while (true) {
                    auto m { r };
                    if (2 * r + 1 < n && this->cmp(nodes[2 * r + 1]->data(), nodes[m]->data())) {
                        m = 2 * r + 1;
                    }
                    if (2 * r + 2 < n && this->cmp(nodes[2 * r + 2]->data(), nodes[m]->data())) {
                        m = 2 * r + 2;
                    }
                    if (m == r) {
                        break;
                    }
                    std::swap(nodes[r]->data(), nodes[m]->data());
                    r = m;
                }
                if (r == 0) {
                    break;
                }
            }
            for (auto r { (n - 2) / 2 }; ; --r) {
                if (2 * r + 2 < n) {
                    nodes[r]->right() = nodes.pop_back();
                    nodes[r]->right()->parent() = nodes[r];
                }
                nodes[r]->left() = nodes.pop_back();
                nodes[r]->left()->parent() = nodes[r];
                nodes[r]->data().npl = npl(nodes[r]->left()) + 1;
                if (r == 0) {
                    break;
                }
            }
        }
        H = nodes.pop_back();
    }

    size_t size() const override {
        return H.size();
    }

    const T& top() const override {
        return H.root()->data().value;
    }

    void merge(LeftistHeap<T>&& other) {
        H = merge(std::move(H), std::move(other.H));
    }

    void push(const T& e) override {
        H = merge(std::move(H), BinaryTreeNodeInst<Element>::make(e));
    }

    void push(T&& e) override {
        H = merge(std::move(H), BinaryTreeNodeInst<Element>::make(std::move(e)));
    }

    T pop() override {
        T e { std::move(H.root()->data().value) };
        H = merge(std::move(H.root()->left()), std::move(H.root()->right()));
        return e;
    }

    template <typename T1>
    friend std::ostream& operator<<(std::ostream& os, const LeftistHeap<T1>& H);

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const LeftistHeap<T>& H) {
    os << "Heap(" << H.H << ")";
    return os;
}

}