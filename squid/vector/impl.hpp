//
// Created by Sam Sleight on 15/01/2016.
//

#ifndef SQUID_VECTOR_IMPL_HPP
#define SQUID_VECTOR_IMPL_HPP

namespace squid {

template<std::size_t N, typename T>
class Vector;

namespace impl {

template<typename V, typename Idxs>
struct vec_idx_impl;

template<std::size_t Size, typename T, std::size_t... Idxs>
struct vec_idx_impl<Vector<Size, T>, std::index_sequence<Idxs...>> {
    template<typename U>
    static constexpr auto dot(const Vector<Size, T>& lhs, const Vector<Size, U>& rhs) {
        return ((lhs.get(Idxs) * rhs.get(Idxs)) + ...);
    };

    template<typename U>
    static constexpr auto scalar_mul(const U& scalar, const Vector<Size, T>& vector) {
        return Vector<Size, decltype(scalar * T())>((scalar * vector.get(Idxs))...);
    };

    template<typename F, typename R>
    struct map_impl {
        static constexpr auto map(const Vector<Size, T>& vec, F f) {
            return Vector<Size, R>(f(vec.get(Idxs))...);
        };
    };

    template<typename F>
    struct map_impl<F, void> {
        static constexpr void map(const Vector<Size, T>& vec, F f) {
            (f(vec.get(Idxs)), ...);
        }
    };
};

}

}

#endif //SQUID_VECTOR_IMPL_HPP
