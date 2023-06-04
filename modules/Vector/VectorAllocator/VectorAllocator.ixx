export module Vector.VectorAllocator;

export import Vector.VectorAllocator.AbstractVectorAllocator;
import std;

export namespace dslab {

template <size_t D> requires (D > 0)
class VectorAllocatorAP : public AbstractVectorAllocator {
protected:
    size_t expand(size_t capacity, size_t size) const override {
        return capacity + D;
    }
    size_t shrink(size_t capacity, size_t size) const override {
        return capacity;
    }

public:
    std::string type_name() const override {
        return std::format("C -> C + {}", D);
    }
};

template <typename Q> requires (Q::num > Q::den)
class VectorAllocatorGP : public AbstractVectorAllocator {
protected:
    size_t expand(size_t capacity, size_t size) const override {
        size_t newCapacity { capacity * Q::num / Q::den };
        return std::max(newCapacity, capacity + 1);
    }
    size_t shrink(size_t capacity, size_t size) const override {
        return capacity;
    }

public:
    std::string type_name() const override {
        if constexpr (Q::den == 1) {
            return std::format("C -> C * {}", Q::num);
        } else {
            return std::format("C -> C * {}/{}", Q::num, Q::den);
        }
    }
};

}