#pragma once

namespace Mino
{

class Asset
{
public:
    Asset() = default;
    Asset(Asset const&) = default;
    Asset(Asset&&) = default;
    virtual ~Asset() {}

    Asset& operator=(Asset const&) = default;
    Asset& operator=(Asset&&) = default;
};

} // namespace Mino
