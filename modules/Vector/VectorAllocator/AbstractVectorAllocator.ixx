export module Vector.VectorAllocator.AbstractVectorAllocator;

import Framework.Algorithm;

export namespace dslab {

class AbstractVectorAllocator : public Algorithm<size_t, size_t, size_t> {
protected:
    virtual size_t expand(size_t capacity, size_t size) const = 0;
    virtual size_t shrink(size_t capacity, size_t size) const = 0;
public:
    size_t operator()(size_t capacity, size_t size) override {
        if (capacity <= size) {
            return expand(capacity, size);
        } else {
            return shrink(capacity, size);
        }
    }
};

}