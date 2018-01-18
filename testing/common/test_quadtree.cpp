#include "quadtree.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

using namespace Mino;

class QuadtreeTests : public ::testing::Test
{
public:
    using Quadtree = Quadtree<int>;

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
    auto node = Quadtree::Node(1, -1);
    ASSERT_TRUE(tree.insert(node));
}

TEST_F(QuadtreeTests, CanInsertMultipleElements)
{
    for (int i = 0; i < 5000; ++i)
    {
        auto node = Quadtree::Node(i, i % 20);
        ASSERT_TRUE(tree.insert(node));
    }
}

TEST_F(QuadtreeTests, CanFindNodes)
{
    for (int i = 0; i < 500; ++i)
    {
        auto node = Quadtree::Node(i, i % 20);
        ASSERT_TRUE(tree.insert(node));
    }

    for (int i = 0; i < 500; ++i)
    {
        auto node = Quadtree::Node(i, i % 20);
        ASSERT_TRUE(tree.contains(node));
    }

    for (int i = 1; i < 500; ++i)
    {
        auto node = Quadtree::Node(-i, -i % 20);
        ASSERT_FALSE(tree.contains(node));
    }
}

TEST_F(QuadtreeTests, CanMoveElement)
{
    Quadtree::Node from{10, 0};
    Quadtree::Node to{20, 25};
    tree.insert(from);

    ASSERT_TRUE(tree.contains(from));
    ASSERT_FALSE(tree.contains(to));

    tree.move(from, to);

    ASSERT_FALSE(tree.contains(from));
    ASSERT_TRUE(tree.contains(to));
}

TEST_F(QuadtreeTests, CanEraseElement)
{
    std::vector<Quadtree::Node> nodes{{1, 2}, {123, 54}, {12, 34}, {54, 56}};
    for (int i = 0; i < 200; ++i)
    {
        nodes.push_back({(float)i, (float)i});
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
    Quadtree::Node rangeCenter{5, 5};
    std::vector<Quadtree::Node> nodesInRange{{4, 5}, {5, 4}, {6, 5}, {5.7f, 4.8f}};
    std::vector<Quadtree::Node> nodesOutRange{{0, 0}, {10, 11}};

    for (auto i = nodesInRange.begin(); i != nodesInRange.end(); ++i)
    {
        tree.insert(*i);
    }
    for (auto i = nodesOutRange.begin(); i != nodesOutRange.end(); ++i)
    {
        tree.insert(*i);
    }

    auto result = tree.queryRange(BoundingBox{rangeCenter.pos, 2});

    for (auto i = nodesInRange.begin(); i != nodesInRange.end(); ++i)
    {
        ASSERT_TRUE(std::find(result.begin(), result.end(), *i) != result.end());
    }
    for (auto i = nodesOutRange.begin(); i != nodesOutRange.end(); ++i)
    {
        ASSERT_TRUE(std::find(result.begin(), result.end(), *i) == result.end());
    }
}
