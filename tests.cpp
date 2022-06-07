#include <gtest/gtest.h>
#include "matrix.h"

TEST(test_matrix, add_erase_elements)
{
    Matrix<int, -1> matrix;
    matrix[2][2] = 0;
    matrix[3][2] = 1;
    matrix[4][5] = 6;
    matrix[3][2] = -1;

    EXPECT_EQ(matrix[2][2], 0);
    EXPECT_EQ(matrix[3][2], -1);
    EXPECT_EQ(matrix[4][5], 6);
    EXPECT_EQ(matrix[1][1], -1);
}

TEST(test_matrix, size)
{
    Matrix<int, -1> matrix;

    EXPECT_EQ(matrix.size(), 0);

    int a = matrix[30][30];
    EXPECT_EQ(matrix.size(), 0);

    matrix[2][2] = 0;
    matrix[3][2] = 1;
    matrix[4][5] = 6;

    EXPECT_EQ(matrix.size(), 3);

    matrix[3][2] = -1;

    EXPECT_EQ(matrix.size(), 2);
}