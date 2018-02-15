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
    const auto json = "{\"color\": \"red\",\"size\": 2\n}";

    auto result = Json::parse<Apple>(json);

    EXPECT_EQ(result.color, "red");
    EXPECT_EQ(result.size, 2);
}
