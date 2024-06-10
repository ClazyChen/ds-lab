#pragma once

#include "Stack.hpp"

template <typename T, template <typename> typename S>
requires std::is_base_of_v<dslab::stack::AbstractStack<T>, S<T>> && (!std::is_base_of_v<dslab::stack::MinStack<T>, S<T>>)
struct std::formatter<S<T>> {
    // output format : [(n = size) / ..., top>
    
    bool m_printSize { false };
    bool m_printType { false };
    
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it { ctx.begin() }, end { ctx.end() };
        // options:
        // - 's' : print the size of the stack
        // - 't' : print the type of the stack

        while (it != end) {
            if (*it == 's') {
                m_printSize = true;
            } else if (*it == 't') {
                m_printType = true;
            } else if (*it == '}') {
                break;
            }
            ++it;
        }
        return it;
    }

    template <typename FormatContext>
    auto format(const S<T>& s, FormatContext& ctx) const {
        std::string result { "[" };
        if (!s.empty()) {
            if (m_printSize) {
                result += std::format("(n = {}) ", s.size());
            } else if (s.size() > 1) {
                result += "..., ";
            }
            result += std::format("{}", s.top());
        }
        result += ">";
        if (m_printType) {
            result += std::format(" [{}]", s.type_name());
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};