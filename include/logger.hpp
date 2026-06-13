#pragma once

#include <memory>
#include <string>

namespace spdlog { class logger; }

namespace utils {

class Logger {
public:
    static Logger& instance();

    void init(std::string name = "calculator",
              std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    void warn(const std::string& msg);
    void error(const std::string& msg);
    void debug(const std::string& msg);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;
    ~Logger() = default;

    std::shared_ptr<spdlog::logger> impl_;
};

} // namespace utils
