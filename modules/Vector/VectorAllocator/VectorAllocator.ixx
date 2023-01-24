export module Vector.VectorAllocator;

export import Vector.VectorAllocator.AbstractVectorAllocator;

export namespace dslab {

template <size_t D> requires (D > 0)
class VectorAllocatorAP : public AbstractVectorAllocator {
protected:
    int expand(int capacity, int size) const override {
        return capacity + D;
    }
    int shrink(int capacity, int size) const override {
        return capacity;
    }
};

template <typename Q> requires (Q::num > Q::den)
class VectorAllocatorGP : public AbstractVectorAllocator {
protected:
    int expand(int capacity, int size) const override {
        int newCapacity = capacity * Q::num / Q::den;
        return newCapacity > capacity ? newCapacity : capacity + 1;
    }
    int shrink(int capacity, int size) const override {
        return capacity;
    }
};

}