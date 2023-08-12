export module DisjointSet;

export import DisjointSet.DisjointSetFind;
export import DisjointSet.DisjointSetUnion;

import Vector;
import std;

export namespace dslab {

template <typename Find = DisjointSetFindPathCompression, template<typename> typename Union = DisjointSetUnionByRank>
requires std::is_base_of_v<AbstractDisjointSetFind, Find> && std::is_base_of_v<AbstractDisjointSetUnion<DisjointSetFindBasic>, Union<DisjointSetFindBasic>>
class DisjointSet {
    Vector<Rank> m_parent;
    Find m_find;
    Union<Find> m_union;

public:
    DisjointSet() = default;
    DisjointSet(size_t size) : m_parent(size) {
        for (Rank i { 0 }; i < size; ++i) {
            m_parent[i] = i;
        }
    }

    Rank find(Rank x) {
        return m_find(x, m_parent);
    }

    void union_(Rank x, Rank y) {
        m_union(x, y, m_parent);
    }

    bool connected(Rank x, Rank y) {
        return find(x) == find(y);
    }

    size_t size() const {
        return m_parent.size();
    }

    void make() {
        m_parent.push_back(m_parent.size());
    }

};

}