#include "services.h"

using namespace Mino;

Services::ServiceMap Services::services = Services::ServiceMap{
    {typeid(ILogService).hash_code(), LogService::create()},
    {typeid(ITimeService).hash_code(), TimeService::create()},
};
