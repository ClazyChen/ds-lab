import Framework;
import Vector;
import std;

using namespace dslab;
using namespace std;

template <typename T>
class VectorUnique : public Algorithm<void(Vector<T>&)> {};

template <typename T>
class VectorUniqueBasic : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        for (Rank r { 0 }; r < V.size(); ++r) {
            V.resize(remove(begin(V) + r + 1, end(V), V[r]) - begin(V));
        }
    }
    string type_name() const override {
        return "Unique one by one (Remove Erase Idiom)";
    }
};

template <typename T>
class VectorUniqueBasic2 : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        for (Rank r { 0 }; r < V.size(); ++r) {
            Rank s { r };
            while (s = find(begin(V) + r + 1, end(V), V[r]) - begin(V), s < V.size()) {
                V.remove(s);
            }
        }
    }
    string type_name() const override {
        return "Unique one by one (find and remove)";
    }
};

template <typename T>
class VectorUniqueFSP : public VectorUnique<T> {
public:
    void operator()(Vector<T>& V) override {
        Rank r { 0 }, s { 0 };
        while (++s < V.size()) {
            if (V[r] != V[s]) {
                V[++r] = move(V[s]);
            }
        }
        V.resize(++r);
    }
    string type_name() const override {
        return "Unique globally   (Fast & Slow Ptr / Sorted only)";
    }
};

template <typename T>
class VectorUniqueSort : public VectorUnique<T> {
    struct Item {
        T value;
        Rank rank;
        bool operator==(const Item& rhs) const {
            return value == rhs.value;
        }
        auto operator<=>(const Item& rhs) const {
            return value <=> rhs.value;
        }
    };
    Vector<Item> W;
    void moveToW(Vector<T>& V) {
        W.resize(V.size());
        for (Rank r { 0 }; r < V.size(); ++r) {
            W[r].value = move(V[r]);
            W[r].rank = r;
        }
    }
    void moveToV(Vector<T>& V) {
        V.resize(W.size());
        transform(begin(W), end(W), begin(V), [](const Item& item) {
            return move(item.value);
            });
    }
public:
    void operator()(Vector<T>& V) override {
        moveToW(V);
        stable_sort(begin(W), end(W));
        W.resize(unique(begin(W), end(W)) - begin(W));
        sort(begin(W), end(W), [](const Item& lhs, const Item& rhs) {
            return lhs.rank < rhs.rank;
            });
        moveToV(V);
    }
    string type_name() const override {
        return "Unique globally   (Sort First)";
    }
};

class VectorUniqueTestProblem : public Algorithm<size_t()> {
protected:
    Vector<int> V;
public:
    void initialize(const Vector<int>& V) {
        this->V = V;
    }
};

template <typename Unique>
    requires is_base_of_v<VectorUnique<int>, Unique>
class VectorUniqueTest : public VectorUniqueTestProblem {
    Unique unique;
public:
    size_t operator()() override {
        unique(V);
        return V.size();
    }
    string type_name() const override {
        return unique.type_name();
    }
};

vector testData { 10, 100, 1000, 10000 };

TestFramework<VectorUniqueTestProblem,
    VectorUniqueTest<VectorUniqueBasic<int>>,
    VectorUniqueTest<VectorUniqueBasic2<int>>,
    VectorUniqueTest<VectorUniqueSort<int>>> test;

void testCase(function<Vector<int>(size_t)> generator) {
    for (auto n : testData) {
        cout << format("n = {:6d} ", n) << endl;
        Vector<int> V { generator(n) };
        test.run([&V](auto& problem) { problem.initialize(V); });
        test();
    }
}

int main() {
    cout << "worst case scenario" << endl;
    testCase([](size_t n) {
        Vector<int> V(n);
        iota(begin(V), end(V), 0);
        return V;
    });
    cout << "best case scenario" << endl;
    testCase([](size_t n) { return Vector<int>(n); });
    return 0;
}

