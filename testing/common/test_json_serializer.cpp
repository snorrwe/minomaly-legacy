#include "json_serializer.h"
#include "gtest/gtest.h"

using namespace Mino;

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
    const auto json = "{\"color\": \"red\",\"size\": -25\n}";

    auto result = Json::parse<Apple>(json);

    EXPECT_EQ(result.color, "red");
    EXPECT_EQ(result.size, -25);
}

TEST_F(JsonSerializerTests, ThrowsOnIncompleteJson)
{
    const auto json = "{\"color\": \"red\"}";
    EXPECT_THROW(Json::parse<Apple>(json), Json::ParseError);
}

TEST_F(JsonSerializerTests, ThrowsParseErrorOnInvalidJson)
{
    auto json = "{asd \"color\": \"red\",\"size\": -25\n}";
    EXPECT_THROW(Json::parse<Apple>(json), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -25\n}";
    EXPECT_THROW(Json::parse<Apple>(json), Json::ParseError);

    json = "{\"color\"asd: \"red\",\"size\": -2asd5\n}";
    EXPECT_THROW(Json::parse<Apple>(json), Json::ParseError);
}
