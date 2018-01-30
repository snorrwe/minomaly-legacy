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
    auto m = Matrix{2, 3};
    std::vector<std::vector<float>> matrix{{1, 2, 3}, {4, 5, 6}};
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            m[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(m[i][j], matrix[i][j]);
        }
    }
}

TEST_F(TestMatrices, DotReturnsCorrectDimensions)
{
    auto lhs = Matrix(1, 2);
    auto rhs = Matrix(2, 3);
    auto result = lhs * rhs;
    ASSERT_EQ(result.columns(), lhs.columns());
    ASSERT_EQ(result.rows(), rhs.rows());
}

TEST_F(TestMatrices, CanInitialize)
{
    std::vector<std::vector<float>> m{{1, 2, 3}, {4, 5, 6}};
    auto matrix = Matrix(m);
    ASSERT_EQ(matrix.columns(), 2);
    ASSERT_EQ(matrix.rows(), 3);
    for (int i = 0; i < matrix.columns(); ++i)
    {
        for (int j = 0; j < matrix.rows(); ++j)
        {
            ASSERT_DOUBLE_EQ(matrix[i][j], m[i][j]);
        }
    }
}

TEST_F(TestMatrices, SimpleSubtractTest)
{
    auto lhs = Matrix({{1, 2}, {2, 1}});
    auto rhs = Matrix({{0, 1}, {1, 0}});
    auto result = lhs - rhs;
    ASSERT_EQ(result.columns(), 2);
    ASSERT_EQ(result.rows(), 2);
    for (int i = 0; i < result.columns(); ++i)
    {
        for (int j = 0; j < result.rows(); ++j)
        {
            ASSERT_EQ(result[i][j], 1);
        }
    }
}

TEST_F(TestMatrices, SimpleAddTest)
{
    auto lhs = Matrix({{1, 2}, {2, 1}});
    auto rhs = Matrix({{1, 0}, {0, 1}});
    auto result = lhs + rhs;
    ASSERT_EQ(result.columns(), 2);
    ASSERT_EQ(result.rows(), 2);
    for (int i = 0; i < result.columns(); ++i)
    {
        for (int j = 0; j < result.rows(); ++j)
        {
            ASSERT_EQ(result[i][j], 2);
        }
    }
}

TEST_F(TestMatrices, SimpleMultiplyTest)
{
    auto rhs = Matrix({{1, 1}, {1, 1}});
    auto result = 3 * rhs;
    ASSERT_EQ(result.columns(), 2);
    ASSERT_EQ(result.rows(), 2);
    for (int i = 0; i < result.columns(); ++i)
    {
        for (int j = 0; j < result.rows(); ++j)
        {
            ASSERT_EQ(result[i][j], 3);
        }
    }
    auto other = rhs * 3;
    ASSERT_TRUE(result == other);
}

TEST_F(TestMatrices, SimpleEqualityTest)
{
    auto lhs = Matrix({{1, 1}, {1, 1}});
    auto rhs = Matrix({{1, 1}, {1, 1}});
    ASSERT_TRUE(lhs == rhs);
    rhs = Matrix({{1, 0}, {1, 1}});
    ASSERT_FALSE(lhs == rhs);
    rhs = Matrix({{1, 1, 1}, {1, 1, 1}});
    ASSERT_FALSE(lhs == rhs);
}

TEST_F(TestMatrices, SimpleTransposeTest)
{
    auto m = Matrix({{1, 2}});
    auto actual = Matrix::transpose(m);
    std::vector<std::vector<float>> v{{1.0}, {2.0}};
    auto expected = Matrix(v);
    ASSERT_TRUE(actual == expected);
}

TEST_F(TestMatrices, SimpleTransposeTest2)
{
    auto m = Matrix({{1, 2}, {3, 4}});
    auto actual = Matrix::transpose(m);
    auto expected = Matrix({{1, 3}, {2, 4}});
    ASSERT_TRUE(actual == expected);
}

TEST_F(TestMatrices, UnitMatrixIsCorrectSizeAndHasOnlyOnes)
{
    auto matrix = Matrix::uniform(3, 2);
    ASSERT_EQ(matrix.columns(), 3);
    ASSERT_EQ(matrix.rows(), 2);
    for (auto i = matrix.begin(); i != matrix.end(); ++i)
        ASSERT_EQ(*i, 1);
}

TEST_F(TestMatrices, CanCreateWithSingleVector)
{
    auto matrix = Matrix({1.0, 2.0, 5.0, 3.0, 4.0, 6.0}, 3, 2);
    ASSERT_EQ(matrix.columns(), 3);
    ASSERT_EQ(matrix.rows(), 2);
    ASSERT_EQ(matrix[0][0], 1);
    ASSERT_EQ(matrix[0][1], 3);
    ASSERT_EQ(matrix[1][0], 2);
    ASSERT_EQ(matrix[1][1], 4);
    ASSERT_EQ(matrix[2][0], 5);
    ASSERT_EQ(matrix[2][1], 6);
}

TEST_F(TestMatrices, CanCreateWithSingleVector_Larger)
{
    auto matrix = Matrix({1.0, 2.0, 5.0, 3.0, 4.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0}, 3, 4);
    ASSERT_EQ(matrix.columns(), 3);
    ASSERT_EQ(matrix.rows(), 4);
    /*
      Values hold their position
      Ie. matrix[0][0] is the bottom left item
      and matrix[2][3] is the top right item
      if the matrix is idented "properly"
  */
    EXPECT_EQ(matrix[0][0], 1);
    EXPECT_EQ(matrix[0][1], 3);
    EXPECT_EQ(matrix[0][2], 7);
    EXPECT_EQ(matrix[0][3], 10);
    EXPECT_EQ(matrix[1][0], 2);
    EXPECT_EQ(matrix[1][1], 4);
    EXPECT_EQ(matrix[1][2], 8);
    EXPECT_EQ(matrix[1][3], 11);
    EXPECT_EQ(matrix[2][0], 5);
    EXPECT_EQ(matrix[2][1], 6);
    EXPECT_EQ(matrix[2][2], 9);
    EXPECT_EQ(matrix[2][3], 12);
}

TEST_F(TestMatrices, SimpleMapTest)
{
    auto lhs = Matrix{{1, 2, 3, 4, 5, 6}, 2, 3};
    auto rhs = Matrix{{-1, -2, -3, -4, -5, -6}, 2, 3};
    auto result = Matrix::map(lhs, rhs, [](float l, float r) { return l + r; });

    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.rows(), 3);
    for (auto i = result.begin(); i != result.end(); ++i)
    {
        EXPECT_EQ(*i, 0);
    }
}

TEST_F(TestMatrices, SimpleLengthTests)
{
    auto matrix = Matrix({1, 1, 1, 1}, 1, 4);
    EXPECT_EQ(matrix.length(), 2);

    matrix = Matrix({1, 1, 1, 1}, 4, 1);
    EXPECT_EQ(matrix.length(), 2);
}

TEST_F(TestMatrices, SimpleDotProductTest)
{
    auto m1 = Matrix({1, 2, 3, 4, 5, 6}, 2, 3);
    auto m2 = Matrix({1, 2, 3, 4, 5, 6}, 3, 2);

    auto result = m1 * m2;

    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.rows(), 2);

    EXPECT_EQ(result, Matrix({22, 28, 49, 64}, 2, 2));
}

TEST_F(TestMatrices, SimpleDotPerformanceTest)
{
    int c1 = 1, r1 = 876, r2 = 300;
    auto m1 = Matrix::uniform(c1, r1, 1);
    auto m2 = Matrix::uniform(r1, r2, 1);

    float average = 0.0;
    int times = 5;
    for (int i = 0; i < times; ++i)
    {
        auto start = std::chrono::system_clock::now();
        auto result = m1 * m2;
        auto end = std::chrono::system_clock::now();

        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        average += elapsedMs.count() / float(times);
    }

    std::cout << "Average computational speed of dot products: [" << c1 << "×" << r1 << "] dot ["
              << r1 << "×" << r2 << "] : " << average << "ms" << std::endl;
    ASSERT_LE(average, 100);
}
