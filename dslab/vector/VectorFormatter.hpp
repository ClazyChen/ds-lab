#pragma once

#include "Vector.hpp"

template <typename T, template <typename> typename V>
requires std::is_base_of_v<dslab::vector::AbstractVector<T>, V<T>>
struct std::formatter<V<T>> {
    // output format : [a1, a2, a3, ...]
    
    std::size_t m_maxElements { 16uz }; // default value
    bool m_printSize { false };
    bool m_printCapacity { false };
    
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it { ctx.begin() }, end { ctx.end() };
        // options:
        // - 'e[N]' : only print the first N elements, where N is a non-negative integer
        // - 'E' : print all elements
        // - 's' : print the size of the vector
        // - 'c' : print the capacity of the vector

        while (it != end) {
            if (*it == 'e') {
                m_maxElements = 0;
                ++it;
                while (it != end && std::isdigit(*it)) {
                    m_maxElements = m_maxElements * 10 + (*it - '0');
                    ++it;
                }
            }
            if (*it == 'E') {
                m_maxElements = std::numeric_limits<std::size_t>::max();
            } else if (*it == 's') {
                m_printSize = true;
            } else if (*it == 'c') {
                m_printCapacity = true;
            } else if (*it == '}') {
                break;
            }
            ++it;
        }

        return it;
    }

    template <typename FormatContext>
    auto format(const V<T>& v, FormatContext& ctx) const {
        std::string result { "[" };
        std::size_t count { 0 };
        for (auto it { std::begin(v) }; it != std::end(v) && count < m_maxElements; ++it) {
            if (count > 0) {
                result += ", ";
            }
            result += std::format("{}", *it);
            ++count;
        }
        if (count < v.size()) {
            result += ", ...";
        }
        result += "]";
        if (m_printSize && m_printCapacity) {
            result += std::format(" ({} / {})", v.size(), v.capacity());
        } else {
            if (m_printSize) {
                result += std::format(" (n: {})", v.size());
            }
            if (m_printCapacity) {
                result += std::format(" (m: {})", v.capacity());
            }
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};