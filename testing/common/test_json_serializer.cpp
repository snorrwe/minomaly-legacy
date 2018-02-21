#include "json.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

using namespace Mino;
using namespace std::string_literals;

class JsonSerializerTests : public ::testing::Test
{
public:
};

struct Apple
{
    std::string color;
    int size;

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Apple::size, "size"),
                               Json::property(&Apple::color, "color"));
    }
};

/*TEST_F(JsonSerializerTests, CanSerializeObject)
{
    // toJson
}*/

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
