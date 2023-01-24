export module Vector.VectorAllocator.AbstractVectorAllocator;

import Framework.Algorithm;

export namespace dslab {

class AbstractVectorAllocator : public Algorithm<int, int, int> {
protected:
    virtual int expand(int capacity, int size) const = 0;
    virtual int shrink(int capacity, int size) const = 0;
public:
    int operator()(int capacity, int size) override {
        if (capacity <= size) {
            return expand(capacity, size);
        } else {
            return shrink(capacity, size);
        }
    }
};

}