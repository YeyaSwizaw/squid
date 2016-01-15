#include <iostream>

#include "squid/matrix/matrix.hpp"

constexpr double const_div(int x) {
    return x / 1.21;
}

constexpr double const_sub(double x) {
    return x - 100.2;
}

template<std::size_t N>
constexpr auto const_sub_row(squid::Vector<N, double> vec) {
    return vec.map(const_sub);
}

int main(int argc, char* argv[]) {
    // Vector and dot product
    constexpr squid::Vector<4> vec1(1, 3, 5, 6);
    constexpr squid::Vector<4> vec2(2, 1, 3, 9);
    constexpr int dot = vec1.dot(vec2);
    std::cout << dot << std::endl;

    constexpr auto vec_res = (6 * vec1).map(const_div);
    vec_res.map([](auto x) { std::cout << x << ", "; });
    std::cout << std::endl << std::endl;

    constexpr squid::Matrix<3, 2> mat1(
        5, 2,
        1, 2,
        3, 4
    );

    constexpr squid::Matrix<2, 3> mat2(
        1, -2, 6,
        3, 4, -1
    );

    constexpr auto mat_res = (2.2 * mat1 * mat2)
        .rows().map(const_sub_row<3>)
        .append_row(squid::Vector<3, double>(4.13, 1.2, 9.9))
        .push_row(squid::Vector<3, double>(3.14, 2.1, 1.1));

    mat_res.rows().map([](auto row) {
        row.map([](auto x) {
            std::cout << x << ", ";
        });

        std::cout << std::endl;
    });
}