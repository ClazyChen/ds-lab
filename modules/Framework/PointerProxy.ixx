module;
#include <memory>

export module Framework.PointerProxy;

// 为了降低不了解智能指针进行实验的门槛
// 将智能指针和裸指针封装成代理类，允许赋值和隐式转换
// 用户可以当成裸指针使用，但不代表没有所有权问题
// 比如，试图在普通二叉树上做Morris遍历仍然是不可行的

export namespace dslab {

template <typename T>
class UniquePointerProxy {
    std::unique_ptr<T> m_ptr {};
public:
    UniquePointerProxy() = default;
    UniquePointerProxy(const UniquePointerProxy&) = delete;
    UniquePointerProxy& operator=(const UniquePointerProxy&) = delete;
    UniquePointerProxy(UniquePointerProxy&& other) {
        m_ptr = std::move(other.m_ptr);
    }
    UniquePointerProxy& operator=(UniquePointerProxy&& other) {
        m_ptr = std::move(other.m_ptr);
        return *this;
    }
    UniquePointerProxy& operator=(T* ptr) {
        m_ptr.reset(ptr);
        return *this;
    }
    UniquePointerProxy& operator=(std::unique_ptr<T>&& ptr) {
        m_ptr = std::move(ptr);
        return *this;
    }
    T* operator->() const {
        return m_ptr.get();
    }
    operator T* () const {
        return m_ptr.get();
    }
    operator bool() const {
        return m_ptr != nullptr;
    }
    T* get() const {
        return m_ptr.get();
    }
    T* release() {
        return m_ptr.release();
    }
    void swap(UniquePointerProxy& other) {
        m_ptr.swap(other.m_ptr);
    }
};

template <typename T>
class RawPointerProxy {
    T* m_ptr { nullptr };
public:
    RawPointerProxy() = default;
    RawPointerProxy(const RawPointerProxy&) = default;
    RawPointerProxy& operator=(const RawPointerProxy&) = default;
    RawPointerProxy(RawPointerProxy&&) = delete;
    RawPointerProxy& operator=(RawPointerProxy&&) = delete;
    RawPointerProxy& operator=(T* ptr) {
        m_ptr = ptr;
        return *this;
    }
    RawPointerProxy& operator=(const UniquePointerProxy<T>& ptr) {
        m_ptr = ptr.get();
        return *this;
    }
    RawPointerProxy& operator=(const std::unique_ptr<T>& ptr) {
        m_ptr = ptr.get();
        return *this;
    }
    T* operator->() const {
        return m_ptr;
    }
    operator T* () const {
        return m_ptr;
    }
    operator bool() const {
        return m_ptr != nullptr;
    }
    T* get() const {
        return m_ptr;
    }
    void swap(RawPointerProxy& other) {
        std::swap(m_ptr, other.m_ptr);
    }
};

}
