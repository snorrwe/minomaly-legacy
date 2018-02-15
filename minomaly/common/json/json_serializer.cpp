#include "json_serializer.h"

using namespace Mino;
using namespace Mino::Json;

Object JsonParser::parse(std::string const& str)
{
    auto result = Object{};

    state = State::Default;
    auto buffer = std::stringstream{};
    auto currentKey = std::string{};

    auto sstr = std::istringstream{str};
    while (sstr)
    {
        char chr;
        sstr >> chr;

        switch (state)
        {
        case State::Default:
            handleDefault(chr, currentKey, buffer);
            break;
        case State::Object:
            handleObject(chr, currentKey, buffer);
            break;
        case State::Key:
            handleKey(chr, currentKey, buffer);
            break;
        case State::UnknownValue:
            handleUnknownValue(chr, currentKey, buffer);
            break;
        case State::NumberValue:
            handleNumberValue(chr, currentKey, buffer, result);
            break;
        case State::DecimalValue:
            handleDecimalValue(chr, currentKey, buffer, result);
            break;
        case State::StringValue:
            handleStringValue(chr, currentKey, buffer, result);
            break;
        case State::BoolValue:
            throw std::runtime_error("Not implemented");
        case State::Array:
            throw std::runtime_error("Not implemented");
        }
    }
    return result;
}

void JsonParser::handleDefault(char chr, std::string& currentKey, std::stringstream& buffer)
{
    switch (chr)
    {
    case '{':
        state = State::Object;
        break;
    case '[':
        state = State::Array;
        break;
    }
}

void JsonParser::handleObject(char chr, std::string& currentKey, std::stringstream& buffer)
{
    if (chr == '"')
    {
        state = State::Key;
    }
    else if (!currentKey.empty() && chr == ':')
    {
        state = State::UnknownValue;
    }
    else if (chr == '}')
    {
        state = State::Default; // TODO: state.pop
    }
    else if (isWhiteSpace(chr) || chr == ',')
    {
        return;
    }
    else
    {
        throw ParseError(std::string{"Unexpected character: ["} + chr + "] in JSON string!");
    }
}

void JsonParser::handleKey(char chr, std::string& currentKey, std::stringstream& buffer)
{
    if (chr == '"')
    {
        state = State::Object;
        buffer >> currentKey;
        buffer = std::stringstream{};
    }
    else
    {
        buffer << chr;
    }
}

void JsonParser::handleUnknownValue(char chr, std::string& currentKey, std::stringstream& buffer)
{
    if (isNumber(chr) || chr == '-')
    {
        buffer << chr;
        state = State::NumberValue;
    }
    else if (chr == '"')
    {
        state = State::StringValue;
    }
    else if (chr == '{')
    {
        // TODO
        throw std::runtime_error("Not implemented");
    }
    else if (chr == '[')
    {
        // TODO
        throw std::runtime_error("Not implemented");
    }
    else
    {
        throw ParseError(std::string{"Unexpected character: ["} + chr + "] in JSON value string!");
    }
}

void JsonParser::handleNumberValue(char chr, std::string& currentKey, std::stringstream& buffer,
                                   Object& result)
{
    if (isNumber(chr))
    {
        buffer << chr;
    }
    else if (chr == '.')
    {
        state = State::DecimalValue;
        buffer << chr;
    }
    else
    {
        // TODO: assert next character for correctness
        state = State::Object; // TODO: state.pop
        buffer >> result[currentKey].integer;
        buffer = std::stringstream{};
        currentKey.clear();
    }
}

void JsonParser::handleDecimalValue(char chr, std::string& currentKey, std::stringstream& buffer,
                                    Object& result)
{
    if (isNumber(chr))
    {
        buffer << chr;
    }
    else
    {
        state = State::Object; // TODO: state.pop
        buffer >> result[currentKey].decimal;
        buffer = std::stringstream{};
        currentKey.clear();
    }
}

void JsonParser::handleStringValue(char chr, std::string& currentKey, std::stringstream& buffer,
                                   Object& result)
{
    if (chr == '"')
    {
        state = State::Object; // TODO: state.pop
        buffer >> result[currentKey].string;
        buffer = std::stringstream{};
        currentKey.clear();
    }
    else
    {
        buffer << chr;
    }
}
