#pragma once

#include "List.hpp"
#include "../forward_list/ForwardList.hpp"
#include "../static_list/StaticList.hpp"

template <typename T, template <typename> typename L>
requires (std::is_base_of_v<dslab::list::bidirectional_list::AbstractList<T>, L<T>> ||
            std::is_base_of_v<dslab::list::forward_list::AbstractForwardList<T>, L<T>> ||
            std::is_base_of_v<dslab::list::static_list::AbstractStaticList<T>, L<T>>)
struct std::formatter<L<T>> {
    // output format : [a1, a2, a3, ...]

    std::size_t m_maxElements { 16uz }; // default value
    bool m_printSize { false };

    constexpr auto parse(std::format_parse_context& ctx) {
        auto it { ctx.begin() }, end { ctx.end() };
        // options:
        // - 'e[N]' : only print the first N elements, where N is a non-negative integer
        // - 'E' : print all elements
        // - 's' : print the size of the list

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
            } else if (*it == '}') {
                break;
            }
            ++it;
        }

        return it;
    }

    template <typename FormatContext>
    auto format(const L<T>& list, FormatContext& ctx) const {
        std::string result { "[" };
        std::size_t count { 0 };
        for (auto it { std::begin(list) }; it != std::end(list) && count < m_maxElements && count < list.size(); ++it) {
            if (count > 0) {
                result += ", ";
            }
            result += std::format("{}", *it);
            ++count;
        }
        if (count < list.size()) {
            result += ", ...";
        }
        result += "]";
        if (m_printSize) {
            result += std::format(" ({} items)", list.size());
        }
        return std::format_to(ctx.out(), "{}", result);
    }

};