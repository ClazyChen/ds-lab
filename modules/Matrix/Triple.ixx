﻿module;
#include <utility>

export module Triple;

export namespace dslab {

template <typename T>
class Triple {
    size_t m_row;
    size_t m_col;
    T m_value;
public:
    Triple() = default;
    Triple(size_t r, size_t c, const T& v) : m_row(r), m_col(c), m_value(v) {}
    Triple(size_t r, size_t c, T&& v) : m_row(r), m_col(c), m_value(std::move(v)) {}
    size_t row() const { return m_row; }
    size_t col() const { return m_col; }
    T& value() { return m_value; }
    const T& value() const { return m_value; }
};

}