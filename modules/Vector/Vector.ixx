export module Vector;

export import Vector.AbstractVector;
export import Vector.VectorIterator;
export import Vector.VectorAllocator;
import std;

export namespace dslab {

template <typename T, typename Alloc = VectorAllocatorGP<std::ratio<3, 2>>>
    requires std::is_base_of_v<AbstractVectorAllocator, Alloc>
class Vector : public AbstractVector<T> {
    std::unique_ptr<T[]> m_data { nullptr };
    size_t m_capacity { 0 };
    size_t m_size { 0 };
    
    T* data() override { return m_data.get(); }
    const T* data() const override { return m_data.get(); }
public:
    size_t capacity() const override { return m_capacity; }
    size_t size() const override { return m_size; }

    Vector() = default;
    Vector(size_t n) : m_data { std::make_unique<T[]>(n) }, m_capacity { n }, m_size { n } {}
    Vector(size_t n, const T& value) : Vector(n) { std::fill_n(m_data.get(), n, value); }
    Vector(const Vector& rhs) : Vector(rhs.m_size) { std::copy_n(rhs.m_data.get(), rhs.m_size, m_data.get()); }
    Vector(Vector&& rhs) noexcept : m_data { std::move(rhs.m_data) }, m_capacity { rhs.m_capacity }, m_size { rhs.m_size } {
        rhs.m_capacity = 0;
        rhs.m_size = 0;
    }
    Vector(std::initializer_list<T> ilist) : Vector(ilist.size()) {
        std::move(ilist.begin(), ilist.end(), m_data.get());
    }
    virtual ~Vector() = default;

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            Vector tmp { rhs };
            std::swap(m_data, tmp.m_data);
            std::swap(m_capacity, tmp.m_capacity);
            std::swap(m_size, tmp.m_size);
        }
        return *this;
    }

    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            m_data = std::move(rhs.m_data);
            m_capacity = rhs.m_capacity;
            m_size = rhs.m_size;
            rhs.m_capacity = 0;
            rhs.m_size = 0;
        }
        return *this;
    }

    Vector& operator=(std::initializer_list<T> ilist) {
        Vector tmp { ilist };
        std::swap(m_data, tmp.m_data);
        std::swap(m_capacity, tmp.m_capacity);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    void reserve(size_t n) override {
        if (n > m_capacity) {
            auto tmp { std::make_unique<T[]>(n) };
            std::move(m_data.get(), m_data.get() + m_size, tmp.get());
            m_data = std::move(tmp);
            m_capacity = n;
        }
    }

    void resize(size_t n) override {
        if (n > m_capacity) {
            reserve(n);
        }
        m_size = n;
    }

    void resize(size_t n, const T& value) {
        if (n > m_capacity) {
            reserve(n);
        }
        if (n > m_size) {
            std::fill_n(m_data.get() + m_size, n - m_size, value);
        }
        m_size = n;
    }

    Rank insert(Rank r, const T& e) override {
        if (m_size == m_capacity) {
            reserve(Alloc {}(m_capacity, m_size));
        }
        std::move_backward(m_data.get() + r, m_data.get() + m_size, m_data.get() + m_size + 1);
        m_data[r] = e;
        ++m_size;
        return r;
    }

    Rank insert(Rank r, T&& e) override {
        if (m_size == m_capacity) {
            reserve(Alloc {}(m_capacity, m_size));
        }
        std::move_backward(m_data.get() + r, m_data.get() + m_size, m_data.get() + m_size + 1);
        m_data[r] = std::move(e);
        ++m_size;
        return r;
    }

    Rank find(const T& e) const override {
        for (size_t i { 0 }; i < m_size; ++i) {
            if (m_data[i] == e) {
                return i;
            }
        }
        return m_size;
    }

    T remove(Rank r) override {
        T e { std::move(m_data[r]) };
        std::move(m_data.get() + r + 1, m_data.get() + m_size, m_data.get() + r);
        --m_size;
        return e;
    }

    void push_back(const T& e) override {
        if (m_size == m_capacity) {
            reserve(Alloc {}(m_capacity, m_size));
        }
        m_data[m_size++] = e;
    }

    void push_back(T&& e) override {
        if (m_size == m_capacity) {
            reserve(Alloc {}(m_capacity, m_size));
        }
        m_data[m_size++] = std::move(e);
    }

    T pop_back() override {
        return std::move(m_data[--m_size]);
    }
};

template <typename T>
using DefaultVector = Vector<T, VectorAllocatorGP<std::ratio<3, 2>>>;

}