export module Sort.InsertionSort;

import Vector;
import List;
import Sort.AbstractSort;
import std;

export namespace dslab {

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractVector<T>, Linear<T>>
class InsertionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& V) override {
        for (Rank i { 1 }; i < V.size(); ++i) {
            if (Rank j { i }; this->cmp(V[i], V[i - 1])) {
                T tmp { std::move(V[i]) };
                do {
                    V[j] = std::move(V[j - 1]);
                } while (--j > 0 && this->cmp(tmp, V[j - 1]));
                V[j] = std::move(tmp);
            }
        }
    }
public:
    std::string type_name() const override {
        return "Insertion Sort";
    }
};

template <typename T, template<typename> typename Linear>
    requires std::is_base_of_v<AbstractList<T>, Linear<T>>
class ListInsertionSort : public AbstractSort<T, Linear> {
protected:
    void sort(Linear<T>& L) override {
        if (L.empty()) {
            return;
        }
        for (ListNodePos<T> i { L.first()->next() }; i != L.tail(); ) {
            if (auto j { i->prev() }; this->cmp(i->data(), j->data())) {
                i = i->next();
                auto tmp { L.remove(j->next()) };
                while (j != nullptr && this->cmp(tmp, j->data())) {
                    j = j->prev();
                }
                L.insertAsNext(j, std::move(tmp));
            } else {
                i = i->next();
            }
        }
    }
public:
    std::string type_name() const override {
        return "Insertion Sort / List";
    }
};

}