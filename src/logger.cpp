#include <logger.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace utils {

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::init(std::string name, std::string pattern) {
    impl_ = spdlog::stdout_color_mt(name);
    impl_->set_pattern(pattern);
    impl_->set_level(spdlog::level::warn);
}

void Logger::warn(const std::string& msg) {
    if (impl_) impl_->warn(msg);
}

void Logger::error(const std::string& msg) {
    if (impl_) impl_->error(msg);
}

void Logger::debug(const std::string& msg) {
    if (impl_) impl_->debug(msg);
}

} // namespace utils