//
// Created by Sam Sleight on 14/01/2016.
//

#ifndef MATHS_MATRIX_HPP
#define MATHS_MATRIX_HPP

#include "vector.hpp"

namespace squid {

template<std::size_t N, std::size_t M, typename T>
class Matrix;

namespace util {

template<typename M, typename Idxs>
struct mat_idx_impl;

template<std::size_t N, std::size_t M, std::size_t P, typename T, typename Idxs>
struct mat_mul_impl;

template<std::size_t Rows, std::size_t Cols, typename T, std::size_t... Idxs>
struct mat_idx_impl<Matrix<Rows, Cols, T>, std::index_sequence<Idxs...>> {
    static constexpr Vector<Cols, T> row(const Matrix<Rows, Cols, T>& mat, std::size_t n) {
        return squid::Vector<Cols, T>(mat.get((Cols * n) + Idxs)...);
    }

    static constexpr Vector<Rows, T> col(const Matrix<Rows, Cols, T>& mat, std::size_t n) {
        return squid::Vector<Rows, T>(mat.get((Cols * Idxs) + n)...);
    };
};

template<std::size_t N, std::size_t M, std::size_t P, typename T, std::size_t... IdxsX, std::size_t...IdxsY>
struct mat_mul_impl<N, M, P, T, std::pair<std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>>> {
    static constexpr Matrix<N, P, T> mul(const Matrix<N, M, T>& lhs, const Matrix<M, P, T>& rhs) {
        return squid::Matrix<N, P, T>(lhs.row(IdxsX).dot(rhs.col(IdxsY))...);
    }
};

template<std::size_t max, std::size_t x, std::size_t y, typename IdxsX, typename IdxsY>
struct make_mul_idxs_impl;

template<std::size_t max, std::size_t x, std::size_t y, std::size_t... IdxsX, std::size_t... IdxsY>
struct make_mul_idxs_impl<max, x, y, std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>> {
    using result = typename make_mul_idxs_impl<max, x, y - 1, std::index_sequence<x, IdxsX...>, std::index_sequence<y, IdxsY...>>::result;
};

template<std::size_t max, std::size_t x, std::size_t... IdxsX, std::size_t... IdxsY>
struct make_mul_idxs_impl<max, x, 0, std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>> {
    using result = typename make_mul_idxs_impl<max, x - 1, max, std::index_sequence<x, IdxsX...>, std::index_sequence<0, IdxsY...>>::result;
};

template<std::size_t max, std::size_t... IdxsX, std::size_t... IdxsY>
struct make_mul_idxs_impl<max, 0, 0, std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>> {
    using result = std::pair<std::index_sequence<0, IdxsX...>, std::index_sequence<0, IdxsY...>>;
};

template<std::size_t n>
using make_mul_idxs = typename make_mul_idxs_impl<n - 1, n - 1, n - 1, std::index_sequence<>, std::index_sequence<>>::result;

}

template<std::size_t N, std::size_t M, typename T = int>
class Matrix {
public:
    static const std::size_t Rows = N;
    static const std::size_t Cols = M;

private:
    using row_idx_impl = util::mat_idx_impl<Matrix<Rows, Cols, T>, std::make_index_sequence<Rows>>;
    using col_idx_impl = util::mat_idx_impl<Matrix<Rows, Cols, T>, std::make_index_sequence<Cols>>;

    T data[Rows * Cols];

public:
    Matrix() = delete;

    template<typename... U>
    constexpr Matrix(U... args) noexcept : data{args...} {};

    constexpr T get(std::size_t n) const {
        return n < (Rows * Cols) ? data[n] : throw std::invalid_argument("Index must be less than Matrix::(Rows * Cols)");
    };

    constexpr Vector<Cols, T> row(std::size_t n) const {
        return n < Rows ? col_idx_impl::row(*this, n) : throw std::invalid_argument("Row index must be less than Matrix::Rows");
    };

    constexpr Vector<Rows, T> col(std::size_t n) const {
        return n < Cols ? row_idx_impl::col(*this, n) : throw std::invalid_argument("Col index must be less than Matrix::Cols");
    };

    template<std::size_t P>
    constexpr Matrix<Rows, P, T> operator*(const Matrix<Cols, P, T>& other) const {
        using Idxs = util::make_mul_idxs<P>;
        return util::mat_mul_impl<Rows, Cols, P, T, Idxs>::mul(*this, other);
    };
};

}

#endif //MATHS_MATRIX_HPP
