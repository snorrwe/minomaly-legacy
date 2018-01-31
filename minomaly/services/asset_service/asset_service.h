#pragma once
#include "p_service.h"
#include <iostream>
#include <memory>
#include <string>

namespace Mino
{

class IAssetLoader
{
public:
    virtual ~IAssetLoader() {}
};

class IAssetService : public Service
{
public:
    virtual ~IAssetService() {}
};

} // namespace Mino
