#pragma once
#include "log_service.h"
#include "p_service.h"
#include "time_service.h"
#include <map>
#include <memory>

namespace Mino
{

class Services
{
public:
    typedef std::map<size_t, std::shared_ptr<IService>> ServiceMap;

    template <typename TService> static std::shared_ptr<TService> get();
    template <typename TService> static void overrideService(std::shared_ptr<TService>);

    virtual ~Services() {}

private:
    Services() {}

    static ServiceMap services;
};

template <typename TService> static std::shared_ptr<TService> Services::get()
{
    return std::static_pointer_cast<TService>(services[typeid(TService).hash_code()]);
}

template <typename TService>
static void Services::overrideService(std::shared_ptr<TService> service)
{
    services[typeid(TService).hash_code()] = service;
}

} // namespace Mino
