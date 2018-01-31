#pragma once
#include "p_service.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace Mino
{

class ILogService : public IService
{
public:
    virtual ~ILogService() {}

    virtual void error(std::string const& message) = 0;
    virtual void warning(std::string const& message) = 0;
    virtual void debug(std::string const& message) = 0;
    virtual void info(std::string const& message) = 0;

    virtual void setFname(std::string const& fname) = 0;
    virtual std::string getFname() = 0;
};

class LogService : public ILogService
{
public:
    static std::shared_ptr<LogService> create(std::string const& fname = "Debug.log");

    LogService() = default;
    LogService(std::string const& fname);
    virtual ~LogService() {}

    virtual void error(std::string const& message);
    virtual void warning(std::string const& message);
    virtual void debug(std::string const& message);
    virtual void info(std::string const& message);

    virtual void setFname(std::string const& fname);
    virtual std::string getFname();

protected:
    std::string fname = "Debug.log";

private:
    void log(std::string const& mode, std::string const& message);
};

} // namespace Mino
