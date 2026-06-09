#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <math_lib/math_functions.hpp>

class CalculatorApp {
public:
    int run(int argc, char* argv[]);

private:
    std::optional<ml::Calculation> parse_json(const std::string& json_str);
    std::optional<ml::Calculation::OperationType> parse_operation(const std::string& op_str);

    void print_help(const std::string& program_name);

    void print_result(int64_t value);
    void print_error(const std::string& what);
};