module;
#include <memory>
#include <stdexcept>

export module Framework.PointerProxy;

// 为了降低不了解智能指针进行实验的门槛
// 将智能指针和裸指针封装成代理类，允许赋值和隐式转换
// 用户可以当成裸指针使用，但不代表没有所有权问题
// 比如，试图在普通二叉树上做Morris遍历仍然是不可行的

// 三种代理类支持的转换方式包括：
// Unique Proxy -> Raw Proxy -> Const Raw Proxy
// Unique Proxy             -> Raw Pointer
// Raw Proxy               <-> Raw Pointer
// Const Raw Proxy         <-> Const Raw Pointer
// Unique Pointer -> Unique Proxy, Raw Proxy, Const Raw Proxy

export namespace dslab {

template <typename T>
class RawPointerProxy;

template <typename T>
class ConstRawPointerProxy;

template <typename T>
class UniquePointerProxy {
    std::unique_ptr<T> m_ptr {};
public:
    UniquePointerProxy() = default;
    UniquePointerProxy(std::unique_ptr<T>&& ptr) : m_ptr(std::move(ptr)) {}
    UniquePointerProxy(const UniquePointerProxy&) {
        throw std::runtime_error("UniquePointerProxy is not copyable");
    }
    UniquePointerProxy& operator=(const UniquePointerProxy&) {
        throw std::runtime_error("UniquePointerProxy is not copyable");
        return *this;
    }
    UniquePointerProxy(UniquePointerProxy&& other) {
        m_ptr = std::move(other.m_ptr);
    }
    UniquePointerProxy& operator=(UniquePointerProxy&& other) {
        m_ptr = std::move(other.m_ptr);
        return *this;
    }
    ~UniquePointerProxy() = default;

    UniquePointerProxy& operator=(T* ptr) {
        m_ptr.reset(ptr);
        return *this;
    }
    UniquePointerProxy& operator=(std::unique_ptr<T>&& ptr) {
        m_ptr = std::move(ptr);
        return *this;
    }
    T& operator*() const {
        return *m_ptr;
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
    void reset(T* ptr = nullptr) {
        m_ptr.reset(ptr);
    }
    void swap(UniquePointerProxy& other) {
        m_ptr.swap(other.m_ptr);
    }
    void create(auto&&... args) {
        m_ptr = std::make_unique<T>(std::forward<decltype(args)>(args)...);
    }
    static auto make(auto&&... args) {
        return UniquePointerProxy(std::make_unique<T>(std::forward<decltype(args)>(args)...));
    }
    bool operator==(const UniquePointerProxy& other) const {
        return m_ptr.get() == other.m_ptr.get();
    }
    auto operator<=>(const UniquePointerProxy& other) const {
        return m_ptr.get() <=> other.m_ptr.get();
    }
    bool operator==(std::nullptr_t) const {
        return m_ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return m_ptr != nullptr;
    }
    bool operator==(T* ptr) const {
        return m_ptr.get() == ptr;
    }
    bool operator!=(T* ptr) const {
        return m_ptr.get() != ptr;
    }
    bool operator==(const std::unique_ptr<T>& ptr) const {
        return m_ptr.get() == ptr.get();
    }
    bool operator!=(const std::unique_ptr<T>& ptr) const {
        return m_ptr.get() != ptr.get();
    }
    bool operator==(const RawPointerProxy<T>& ptr) const {
        return m_ptr.get() == ptr.get();
    }
    bool operator!=(const RawPointerProxy<T>& ptr) const {
        return m_ptr.get() != ptr.get();
    }
    bool operator==(const ConstRawPointerProxy<T>& ptr) const {
        return m_ptr.get() == ptr.get();
    }
    bool operator!=(const ConstRawPointerProxy<T>& ptr) const {
        return m_ptr.get() != ptr.get();
    }
};

template <typename T>
class RawPointerProxy {
    T* m_ptr { nullptr };
public:
    RawPointerProxy() = default;
    RawPointerProxy(const RawPointerProxy&) = default;
    RawPointerProxy& operator=(const RawPointerProxy&) = default;
    RawPointerProxy(RawPointerProxy&&) = default;
    RawPointerProxy& operator=(RawPointerProxy&&) = default;
    ~RawPointerProxy() = default;

    RawPointerProxy(T* ptr) : m_ptr(ptr) {}
    RawPointerProxy(const UniquePointerProxy<T>& ptr) {
        m_ptr = ptr.get();
    }
    RawPointerProxy(const std::unique_ptr<T>& ptr) {
        m_ptr = ptr.get();
    }
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
    T& operator*() const {
        return *m_ptr;
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
    bool operator==(const RawPointerProxy& other) const {
        return m_ptr == other.m_ptr;
    }
    auto operator<=>(const RawPointerProxy& other) const {
        return m_ptr <=> other.m_ptr;
    }
    bool operator==(std::nullptr_t) const {
        return m_ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return m_ptr != nullptr;
    }
    bool operator==(T* ptr) const {
        return m_ptr == ptr;
    }
    bool operator!=(T* ptr) const {
        return m_ptr != ptr;
    }
    bool operator==(const std::unique_ptr<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const std::unique_ptr<T>& ptr) const {
        return m_ptr != ptr.get();
    }
    bool operator==(const UniquePointerProxy<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const UniquePointerProxy<T>& ptr) const {
        return m_ptr != ptr.get();
    }
    bool operator==(const ConstRawPointerProxy<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const ConstRawPointerProxy<T>& ptr) const {
        return m_ptr != ptr.get();
    }
};


template <typename T>
class ConstRawPointerProxy {
    const T* m_ptr { nullptr };
public:
    ConstRawPointerProxy() = default;
    ConstRawPointerProxy(const ConstRawPointerProxy&) = default;
    ConstRawPointerProxy& operator=(const ConstRawPointerProxy&) = default;
    ConstRawPointerProxy(ConstRawPointerProxy&&) = delete;
    ConstRawPointerProxy& operator=(ConstRawPointerProxy&&) = delete;
    ~ConstRawPointerProxy() = default;
    ConstRawPointerProxy(const T* ptr) : m_ptr(ptr) {}
    ConstRawPointerProxy(const UniquePointerProxy<T>& ptr) {
        m_ptr = ptr.get();
    }
    ConstRawPointerProxy(const RawPointerProxy<T>& ptr) {
        m_ptr = ptr.get();
    }
    ConstRawPointerProxy(const std::unique_ptr<T>& ptr) {
        m_ptr = ptr.get();
    }
    ConstRawPointerProxy& operator=(const T* ptr) {
        m_ptr = ptr;
        return *this;
    }
    ConstRawPointerProxy& operator=(const UniquePointerProxy<T>& ptr) {
        m_ptr = ptr.get();
        return *this;
    }
    ConstRawPointerProxy& operator=(const RawPointerProxy<T>& ptr) {
        m_ptr = ptr.get();
        return *this;
    }
    ConstRawPointerProxy& operator=(const std::unique_ptr<T>& ptr) {
        m_ptr = ptr.get();
        return *this;
    }
    const T* operator->() const {
        return m_ptr;
    }
    const T& operator*() const {
        return *m_ptr;
    }
    operator const T* () const {
        return m_ptr;
    }
    operator bool() const {
        return m_ptr != nullptr;
    }
    operator RawPointerProxy<T>() const {
        return RawPointerProxy<T>(const_cast<T*>(m_ptr));
    }
    const T* get() const {
        return m_ptr;
    }
    void swap(ConstRawPointerProxy& other) {
        std::swap(m_ptr, other.m_ptr);
    }
    bool operator==(const ConstRawPointerProxy& other) const {
        return m_ptr == other.m_ptr;
    }
    auto operator<=>(const ConstRawPointerProxy& other) const {
        return m_ptr <=> other.m_ptr;
    }
    bool operator==(std::nullptr_t) const {
        return m_ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return m_ptr != nullptr;
    }
    bool operator==(const T* ptr) const {
        return m_ptr == ptr;
    }
    bool operator!=(const T* ptr) const {
        return m_ptr != ptr;
    }
    bool operator==(const std::unique_ptr<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const std::unique_ptr<T>& ptr) const {
        return m_ptr != ptr.get();
    }
    bool operator==(const UniquePointerProxy<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const UniquePointerProxy<T>& ptr) const {
        return m_ptr != ptr.get();
    }
    bool operator==(const RawPointerProxy<T>& ptr) const {
        return m_ptr == ptr.get();
    }
    bool operator!=(const RawPointerProxy<T>& ptr) const {
        return m_ptr != ptr.get();
    }
};

}
