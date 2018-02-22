#include "json.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using namespace Mino;
using namespace std::string_literals;

class JsonSerializerTests : public ::testing::Test
{
protected:
};

struct Apple
{
    std::string color = "";
    int size = 0;

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Apple::color, "color"),
                               Json::property(&Apple::size, "size"));
    }
};

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

TEST(TestJsonSetter, CanFindTypeByName)
{
    auto apple = Apple{};
    Json::Private::setProperty(apple, "color", "blue"s);
    Json::Private::setProperty(apple, "size", 5);
    EXPECT_EQ(apple.size, 5);
    EXPECT_EQ(apple.color, "blue"s);
}

TEST_F(JsonSerializerTests, CanReadJsonIntoObject)
{
    const auto json = "{\"color\": \"red\",\"size\": -25\n}"s;

    auto result = Json::parse<Apple>(json.begin(), json.end());

    EXPECT_EQ(result.color, "red");
    EXPECT_EQ(result.size, -25);
}

TEST_F(JsonSerializerTests, ThrowsOnIncompleteJson)
{
    const auto json = "{\"color\": \"red\"}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);
}

TEST_F(JsonSerializerTests, ThrowsParseErrorOnInvalidJson)
{
    auto json = "{asd \"color\": \"red\",\"size\": -25\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -25\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -2asd5\n}"s;
    EXPECT_THROW(Json::parse<Apple>(json.begin(), json.end()), Json::ParseError);
}
