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
struct mat_pair_impl;

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

    static constexpr auto from_row_vectors(const Vector<Cols, T>& row) {
        return Matrix<1, Cols, T>(row.get(Idxs)...);
    }

    template<typename... V>
    static constexpr auto from_row_vectors(const Vector<Cols, T>& row, V... vecs) {
        return Matrix<Rows - 1, Cols, T>::from_row_vectors(vecs...).push_row(row);
    }

    static constexpr auto from_col_vectors(const Vector<Rows, T>& col) {
        return Matrix<Rows, 1, T>(col.get(Idxs)...);
    }

    template<typename... V>
    static constexpr auto from_col_vectors(const Vector<Rows, T>& col, V... vecs) {
        return Matrix<Rows, Cols - 1, T>::from_col_vectors(vecs...).push_col(col);
    }

    template<typename F, typename R>
    struct map_impl {
        static constexpr auto row_map(const Matrix<Rows, Cols, T>& mat, F f) {
            return Matrix<Rows, Cols, typename R::Type>::from_row_vectors(f(mat.row(Idxs))...);
        }

        static constexpr auto col_map(const Matrix<Rows, Cols, T>& mat, F f) {
            return Matrix<Rows, Cols, typename R::Type>::from_col_vectors(f(mat.col(Idxs))...);
        }
    };

    template<typename F>
    struct map_impl<F, void> {
        static constexpr void row_map(const Matrix<Rows, Cols, T>& mat, F f) {
            (f(mat.row(Idxs)) , ...);
        }

        static constexpr void col_map(const Matrix<Rows, Cols, T>& mat, F f) {
            (f(mat.col(Idxs)) , ...);
        }
    };

    template<typename F>
    static constexpr auto row_map(const Matrix<Rows, Cols, T>& mat, F f) {
        using R = decltype(f(mat.row(0)));
        return map_impl<F, R>::row_map(mat, f);
    }

    template<typename F>
    static constexpr auto col_map(const Matrix<Rows, Cols, T>& mat, F f) {
        using R = decltype(f(mat.col(0)));
        return map_impl<F, R>::col_map(mat, f);
    }
};

template<std::size_t I, std::size_t N, std::size_t M, typename T, typename Idxs, bool done = I == N - 1>
struct push_col_impl;

template<std::size_t I, std::size_t N, std::size_t M, typename T, std::size_t... Idxs>
struct push_col_impl<I, N, M, T, std::index_sequence<Idxs...>, false> {
    static constexpr auto push_col(const Matrix<N, M, T>& matrix, const Vector<N, T>& col) {
        return push_col_impl<I + 1, N, M, T, std::index_sequence<Idxs...>>::push_col(matrix, col).push_row(Vector<M + 1, T>(col.get(I), matrix.row(I).get(Idxs)...));
    }
};

template<std::size_t I, std::size_t N, std::size_t M, typename T, std::size_t... Idxs>
struct push_col_impl<I, N, M, T, std::index_sequence<Idxs...>, true> {
    static constexpr auto push_col(const Matrix<N, M, T>& matrix, const Vector<N, T>& col) {
        return Matrix<1, M + 1, T>(col.get(I), matrix.row(I).get(Idxs)...);
    }
};


template<std::size_t N, std::size_t M, std::size_t P, typename T, std::size_t... IdxsX, std::size_t...IdxsY>
struct mat_pair_impl<N, M, P, T, std::pair<std::index_sequence<IdxsX...>, std::index_sequence<IdxsY...>>> {
    template<typename U>
    static constexpr Matrix<N, P, T> mul(const Matrix<N, M, T>& lhs, const Matrix<M, P, U>& rhs) {
        return squid::Matrix<N, P, decltype(T() * U())>(lhs.row(IdxsX).dot(rhs.col(IdxsY))...);
    }

    static constexpr Matrix<N + 1, M, T> append_row(const Matrix<N, M, T>& matrix, const Vector<M, T>& row) {
        return Matrix<N + 1, M, T>(matrix.get(IdxsX)..., row.get(IdxsY)...);
    };

    static constexpr Matrix<N + 1, M, T> push_row(const Matrix<N, M, T>& matrix, const Vector<M, T>& row) {
        return Matrix<N + 1, M, T>(row.get(IdxsY)..., matrix.get(IdxsX)...);
    };

    static constexpr Matrix<N, M + 1, T> push_col(const Matrix<N, M, T>& matrix, const Vector<N, T>& col) {
        return push_col_impl<0, N, M, T, std::index_sequence<IdxsY...>>::push_col(matrix, col);
    };
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
