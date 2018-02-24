#include "json.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using namespace Mino;
using namespace std::string_literals;

TEST(TestJsonSetter, TestStrEqual)
{
    auto phrase1 = "red";
    auto phrase2 = "red";
    auto phrase3 = "blue";
    auto phrase4 = "red123";
    auto phrase5 = "re";

    EXPECT_TRUE(Json::Private::strEqual(phrase1, phrase2));
    EXPECT_FALSE(Json::Private::strEqual(phrase1, phrase3));
    EXPECT_FALSE(Json::Private::strEqual(phrase1, phrase4));
    EXPECT_FALSE(Json::Private::strEqual(phrase1, phrase5));
}

struct Seed
{
    double radius = 0.0;

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Seed::radius, "radius"));
    }
};

struct Apple
{
    std::string color = "";
    int size = 0;
    Seed seed;

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Apple::color, "color"),
                               Json::property(&Apple::seed, "seed"),
                               Json::property(&Apple::size, "size"));
    }
};

TEST(TestJsonSetter, CanFindTypeByName)
{
    auto apple = Apple{};
    Json::Private::setProperty(apple, "color", "red"s);
    Json::Private::setProperty(apple, "size", 5);
    EXPECT_EQ(apple.size, 5);
    EXPECT_EQ(apple.color, "red"s);
}

class TestJsonParser : public ::testing::Test
{
protected:
};

TEST_F(TestJsonParser, CanReadJsonIntoObject)
{
    const auto json
        = "     {\"color\":\"red\",\"size\": -25\n, \"seed\":   {\"radius\":        -3.14}}"s;

    auto result = Json::parse<Apple>(json.begin(), json.end());

    EXPECT_EQ(result.color, "red");
    EXPECT_EQ(result.size, -25);
    EXPECT_FLOAT_EQ(result.seed.radius, -3.14);
}

TEST_F(TestJsonParser, ThrowsParseErrorOnInvalidJson)
{
    auto json = "{asd \"color\": \"red\",\"size\": -25\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -25\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -2asd5\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);
}

struct AppleTree
{
    std::string id = "";
    std::vector<Apple> apples = {};

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&AppleTree::apples, "apples"),
                               Json::property(&AppleTree::id, "id"));
    }
};

TEST_F(TestJsonParser, CanReadObjectWithVectorOfObjects)
{
    const auto json = "{\"apples\": ["
                      "{\"color\":\"red\",\"size\":0,\"seed\":{\"radius\":0}},"
                      "{\"color\":\"red\",\"size\":1,\"seed\":{\"radius\":1}},"
                      "{\"color\":\"red\",\"size\":2,\"seed\":{\"radius\":2}},"
                      "{\"color\":\"red\",\"size\":3,\"seed\":{\"radius\":3}},"
                      "{\"color\":\"red\",\"size\":4,\"seed\":{\"radius\":4}}"
                      "]}"s;

    auto result = Json::parse<AppleTree>(json.begin(), json.end());

    ASSERT_EQ(result.apples.size(), 5);

    auto i = 0;
    for (auto& apple : result.apples)
    {
        EXPECT_EQ(apple.color, "red");
        EXPECT_EQ(apple.size, i);
        EXPECT_FLOAT_EQ(apple.seed.radius, i);
        ++i;
    }
}

struct Orchid
{
    std::vector<AppleTree> trees = {};

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Orchid::trees, "trees"));
    }
};

TEST_F(TestJsonParser, CanReadVectorOfObjectsWithVectors)
{
    const auto json = "{"
                      "\"trees\":["
                      "  {\"id\":\"tree1\",\"apples\": ["
                      "      {\"color\":\"red\",\"size\":0,\"seed\":{\"radius\":0}},"
                      "      {\"color\":\"red\",\"size\":1,\"seed\":{\"radius\":1}}"
                      "  ]},"
                      "  {\"id\":\"tree2\",\"apples\": ["
                      "      {\"color\":\"red\",\"size\":0,\"seed\":{\"radius\":0}},"
                      "      {\"color\":\"red\",\"size\":1,\"seed\":{\"radius\":1}}"
                      "  ]}"
                      " ]"
                      "}"s;

    auto result = Json::parse<Orchid>(json.begin(), json.end());

    EXPECT_EQ(result.trees.size(), 2);
}

TEST_F(TestJsonParser, RaisesExceptionIfNonExistentPropertyIsRead)
{
    auto json = "{\"color\": \"red\",\"size\": -25\n, \"fakeproperty\": \"asd\"}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::UnexpectedPropertyName);
}
