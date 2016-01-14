#include <iostream>

#include "vector.hpp"
#include "matrix.hpp"

int main(int argc, char* argv[]) {
    // Vector and dot product
    constexpr squid::Vector<4> vec1(1, 3, 5, 6);
    constexpr squid::Vector<4> vec2(2, 1, 3, 9);
    constexpr int dot = vec1.dot(vec2);
    std::cout << dot << std::endl;

    constexpr squid::Matrix<3, 2> mat1(
        5, 2,
        1, 2,
        3, 4
    );

    constexpr auto row = mat1.row(2);
    constexpr int item1 = row.get(0);
    std::cout << item1 << std::endl;

    constexpr auto col = mat1.col(1);
    constexpr int item2 = col.get(1);
    std::cout << item2 << std::endl;

    constexpr squid::Matrix<2, 3> mat2(
        1, -2, 6,
        3, 4, -1
    );

    constexpr auto mat = mat1 * mat2;

    for(int i = 0; i < mat.Rows; ++i) {
        for(int j = 0; j < mat.Cols; ++j) {
            std::cout << mat.row(i).get(j) << ",";
        }

        std::cout << std::endl;
    }
}