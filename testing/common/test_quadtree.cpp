#include "quadtree.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

using namespace Mino;

class QuadtreeTests : public ::testing::Test
{
public:
    void SetUp()
    {
        auto box = BoundingBox({0, 0}, 5000);
        tree = Quadtree(box);
    }

protected:
    Quadtree tree;
};

TEST_F(QuadtreeTests, CanCreate) {}

TEST_F(QuadtreeTests, CanInsert)
{
    auto node = Quadtree::Vector(1, -1);
    ASSERT_TRUE(tree.insert(node));
}

TEST_F(QuadtreeTests, CanInsertMultipleElements)
{
    for (int i = 0; i < 5000; ++i)
    {
        auto node = Quadtree::Vector(i, i % 20);
        ASSERT_TRUE(tree.insert(node));
    }
}

TEST_F(QuadtreeTests, CanFindNodes)
{
    for (int i = 0; i < 5000; ++i)
    {
        auto node = Quadtree::Vector(i, i % 20);
        ASSERT_TRUE(tree.insert(node));
    }

    for (int i = 0; i < 5000; ++i)
    {
        auto node = Quadtree::Vector(i, i % 20);
        ASSERT_TRUE(tree.contains(node));
    }

    for (int i = 1; i < 5000; ++i)
    {
        auto node = Quadtree::Vector(-i, -i % 20);
        ASSERT_FALSE(tree.contains(node));
    }
}

TEST_F(QuadtreeTests, CanMoveElement)
{
    Quadtree::Vector from{10, 0};
    Quadtree::Vector to{20, 25};
    tree.insert(from);
    tree.move(from, to);

    ASSERT_FALSE(tree.contains(from));
    ASSERT_TRUE(tree.contains(to));
}

TEST_F(QuadtreeTests, CanEraseElement)
{
    std::vector<Quadtree::Vector> nodes{{1, 2}, {123, 54}, {12, 34}, {54, 56}};
    for (int i = 0; i < 2000; ++i)
    {
        nodes.push_back({(double)i, (double)i});
    }
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        tree.insert(*i);
    }
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        ASSERT_TRUE(tree.contains(*i));
    }
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        tree.erase(*i);
    }
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        ASSERT_FALSE(tree.contains(*i));
    }
}

TEST_F(QuadtreeTests, CanFindNodesInRange)
{
    Quadtree::Vector rangeCenter{5, 5};
    std::vector<Quadtree::Vector> nodesInRange{{4, 5}, {5, 4}, {6, 5}, {5.7, 4.8}};
    std::vector<Quadtree::Vector> nodesOutRange{{0, 0}, {10, 11}};

    for (auto i = nodesInRange.begin(); i != nodesInRange.end(); ++i)
    {
        tree.insert(*i);
    }
    for (auto i = nodesOutRange.begin(); i != nodesOutRange.end(); ++i)
    {
        tree.insert(*i);
    }

    auto result = tree.queryRange(BoundingBox{rangeCenter, 2});

    for (auto i = nodesInRange.begin(); i != nodesInRange.end(); ++i)
    {
        ASSERT_TRUE(std::find(result.begin(), result.end(), *i) != result.end());
    }
    for (auto i = nodesOutRange.begin(); i != nodesOutRange.end(); ++i)
    {
        ASSERT_TRUE(std::find(result.begin(), result.end(), *i) == result.end());
    }
}
