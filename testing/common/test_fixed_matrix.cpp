#include "matrix.h"
#include "gtest/gtest.h"
#include <chrono>
#include <iostream>

using namespace Mino;

class TestMatrices : public ::testing::Test
{
public:
};

TEST_F(TestMatrices, SimpleCanAssignMembers)
{
    auto m = Matrix<2, 3>{};
    std::vector<std::vector<float>> matrix{{1, 2, 3}, {4, 5, 6}};
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            m.at(i, j) = matrix[i][j];
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            EXPECT_EQ(m.at(i, j), matrix[i][j]);
        }
    }
}
TEST_F(TestMatrices, DotReturnsCorrectDimensions)
{
    auto lhs = Matrix<1, 2>{};
    auto rhs = Matrix<2, 3>{};
    auto result = lhs * rhs;
    EXPECT_EQ(result.columns(), lhs.columns());
    EXPECT_EQ(result.rows(), rhs.rows());
}

TEST_F(TestMatrices, SimpleInitTest)
{
    auto m = Matrix<2, 3>({1, 2, 3, 4, 5, 6});

    EXPECT_EQ(m.at(0, 0), 1);
    EXPECT_EQ(m.at(1, 0), 2);
    EXPECT_EQ(m.at(0, 1), 3);
    EXPECT_EQ(m.at(1, 1), 4);
    EXPECT_EQ(m.at(0, 2), 5);
    EXPECT_EQ(m.at(1, 2), 6);
}

TEST_F(TestMatrices, SimpleDotProductTest)
{
    auto m1 = Matrix<2, 3>({1, 2, 3, 4, 5, 6});
    auto m2 = Matrix<3, 2>({1, 2, 3, 4, 5, 6});

    auto result = m1 * m2;

    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.rows(), 2);

    auto expected = Matrix<2, 2>{{22, 28, 49, 64}};
    EXPECT_EQ(result, expected);
}

TEST_F(TestMatrices, SimpleSubtractTest)
{
    auto lhs = Matrix<2, 2>({1, 2, 2, 1});
    auto rhs = Matrix<2, 2>({0, 1, 1, 0});
    auto result = lhs - rhs;
    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.rows(), 2);
    for (int i = 0; i < result.columns(); ++i)
    {
        for (int j = 0; j < result.rows(); ++j)
        {
            EXPECT_EQ(result.at(i, j), 1);
        }
    }
}

TEST_F(TestMatrices, SimpleAddTest)
{
    auto lhs = Matrix<2, 2>({1, 2, 2, 1});
    auto rhs = Matrix<2, 2>({1, 0, 0, 1});
    auto result = lhs + rhs;
    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.rows(), 2);
    for (int i = 0; i < result.columns(); ++i)
    {
        for (int j = 0; j < result.rows(); ++j)
        {
            EXPECT_EQ(result.at(i, j), 2);
        }
    }
}

TEST_F(TestMatrices, SimpleEqualityTest)
{
    auto lhs = Matrix<2, 2>({1, 1, 1, 1});
    auto rhs = Matrix<2, 2>({1, 1, 1, 1});
    ASSERT_TRUE(lhs == rhs);
    rhs = Matrix<2, 2>({1, 0, 1, 1});
    ASSERT_FALSE(lhs == rhs);
}

TEST_F(TestMatrices, SimpleTransposeTest)
{
    auto m = Matrix<1, 2>({1, 2});
    auto actual = m.transpose();
    auto expected = Matrix<2, 1>({1, 2});
    ASSERT_TRUE(actual == expected);
}
