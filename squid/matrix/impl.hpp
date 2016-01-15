//
// Created by Sam Sleight on 15/01/2016.
//

#ifndef SQUID_MATRIX_IMPL_HPP
#define SQUID_MATRIX_IMPL_HPP

#include "squid/vector/vector.hpp"

namespace squid {

template<std::size_t N, std::size_t M, typename T>
class Matrix;

namespace impl {

template<typename M, typename Idxs>
struct mat_idx_impl;

template<std::size_t N, std::size_t M, std::size_t P, typename T, typename Idxs>
struct mat_mul_impl;

template<std::size_t Rows, std::size_t Cols, typename T, std::size_t... Idxs>
struct mat_idx_impl<Matrix<Rows, Cols, T>, std::index_sequence<Idxs...>> {
static constexpr Vector<Cols, T> row(const Matrix<Rows, Cols, T>& mat, std::size_t n) {
    return squid::Vector<Cols, T>(mat.get((Cols * n) + Idxs)...);
};

static constexpr Vector<Rows, T> col(const Matrix<Rows, Cols, T>& mat, std::size_t n) {
    return squid::Vector<Rows, T>(mat.get((Cols * Idxs) + n)...);
};

template<typename U>
static constexpr auto scalar_mul(const U& scalar, const Matrix<Rows, Cols, T>& matrix) {
    return Matrix<Rows, Cols, decltype(scalar * T())>((scalar * matrix.get(Idxs))...);
}
};

template<std::size_t N, std::size_t M, std::size_t P, typename T, std::size_t... IdxsX, std::size_t...IdxsY>
struct mat_mul_impl<N, M, P, T, std::pair<std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>>> {
template<typename U>
static constexpr Matrix<N, P, T> mul(const Matrix<N, M, T>& lhs, const Matrix<M, P, U>& rhs) {
    return squid::Matrix<N, P, decltype(T() * U())>(lhs.row(IdxsX).dot(rhs.col(IdxsY))...);
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

}

#endif //SQUID_MATRIX_IMPL_HPP
