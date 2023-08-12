export module DisjointSet.DisjointSetUnion;

import Vector;
import Framework.Algorithm;
import DisjointSet.DisjointSetFind;
import std;

export namespace dslab {

template <typename Find>
requires std::is_base_of_v<AbstractDisjointSetFind, Find>
class AbstractDisjointSetUnion : public Algorithm<void(Rank, Rank, Vector<Rank>&)> {
protected:
    Find find;
};

template <typename Find>
class DisjointSetUnionBasic : public AbstractDisjointSetUnion<Find> {
public:
    void operator()(Rank x, Rank y, Vector<Rank>& parent) override {
        Rank xRoot { this->find(x, parent) };
        Rank yRoot { this->find(y, parent) };
        if (xRoot == yRoot) return;
        parent[xRoot] = yRoot;
    }
};

template <typename Find>
class DisjointSetUnionBySize : public AbstractDisjointSetUnion<Find> {
    Vector<size_t> m_size;
    void update(const Vector<Rank>& parent) {
        if (auto cur { m_size.size() }; cur < parent.size()) {
            m_size.resize(parent.size());
            std::fill(std::begin(m_size) + cur, std::end(m_size), 1);
        }
    }
public:
    void operator()(Rank x, Rank y, Vector<Rank>& parent) override {
        update(parent);
        Rank xRoot { this->find(x, parent) };
        Rank yRoot { this->find(y, parent) };
        if (xRoot == yRoot) return;
        if (m_size[xRoot] < m_size[yRoot]) {
            parent[xRoot] = yRoot;
            m_size[yRoot] += m_size[xRoot];
        } else {
            parent[yRoot] = xRoot;
            m_size[xRoot] += m_size[yRoot];
        }
    }
};

template <typename Find>
class DisjointSetUnionByRank : public AbstractDisjointSetUnion<Find> {
    Vector<size_t> m_rank;
    void update(const Vector<Rank>& parent) {
        if (auto cur { m_rank.size() }; cur < parent.size()) {
            m_rank.resize(parent.size());
            std::fill(std::begin(m_rank) + cur, std::end(m_rank), 0);
        }
    }
public:
    void operator()(Rank x, Rank y, Vector<Rank>& parent) override {
        update(parent);
        Rank xRoot { this->find(x, parent) };
        Rank yRoot { this->find(y, parent) };
        if (xRoot == yRoot) return;
        if (m_rank[xRoot] < m_rank[yRoot]) {
            parent[xRoot] = yRoot;
        } else if (m_rank[xRoot] > m_rank[yRoot]) {
            parent[yRoot] = xRoot;
        } else {
            parent[yRoot] = xRoot;
            m_rank[xRoot]++;
        }
    }
};

}