#pragma once

#include "../framework.hpp"

namespace dslab::vector {

class VectorAllocator : public Algorithm<std::size_t(std::size_t, std::size_t)> {
protected:
    virtual std::size_t expand(std::size_t capacity, std::size_t size) const = 0;
    virtual std::size_t shrink(std::size_t capacity, std::size_t size) const {
        return capacity;
    }
public:
    std::size_t operator()(std::size_t capacity, std::size_t size) override {
        if (capacity <= size) {
            return expand(capacity, size);
        } else {
            return shrink(capacity, size);
        }
    }
};

template <std::size_t D> requires (D > 0)
class VectorAllocatorAP : public VectorAllocator {
protected:
    std::size_t expand(std::size_t capacity, std::size_t size) const override {
        return capacity + D;
    }
public:
    std::string type_name() const override {
        return std::format("C -> C + {}", D);
    }
};

template <typename Q> requires (Q::num > Q::den)
class VectorAllocatorGP : public VectorAllocator {
protected:
    std::size_t expand(std::size_t capacity, std::size_t size) const override {
        std::size_t newCapacity { capacity * Q::num / Q::den };
        return std::max(newCapacity, capacity + 1);
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