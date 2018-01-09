#include "log_service.h"

using namespace Mino;

LogService::LogService(std::string const& fname) : fname(fname) {}

void LogService::log(std::string const& mode, std::string const& message)
{
    auto now = std::chrono::system_clock::now();
    auto nowt = std::chrono::system_clock::to_time_t(now);
    std::ofstream f(fname, std::ios::app);
    f << std::ctime(&nowt) << "    [" << mode << "]    " << message << std::endl;
}

void LogService::error(std::string const& message) { log("ERROR", message); }

void LogService::debug(std::string const& message) { log("DEBUG", message); }

void LogService::warning(std::string const& message) { log("WARNING", message); }

void LogService::info(std::string const& message) { log("INFO", message); }

void LogService::setFname(std::string const& f) { fname = f; }

std::string LogService::getFname() { return fname; }

std::shared_ptr<LogService> LogService::create(std::string const& fname)
{
    return std::make_shared<LogService>(fname);
}
