#include <cstdio>
#include <cinttypes>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>
#include <calculator_app.hpp>
#include <math_lib/math_functions.hpp>
#include <logger.hpp>

using json = nlohmann::json;

int CalculatorApp::run(int argc, char* argv[]) {
    if (argc != 2) {
        print_help(argv[0]);
        return 1;
    }

    std::string json_str(argv[1]);
    std::optional<ml::Calculation> calc = parse_json(json_str);

    if (!calc) {
        print_help(argv[0]);
        return 1;
    }

    int64_t result = 0;
    try {
        result = calc.value().calculate();
    } catch (const CalculationError& e) {
        print_error(e.what());
        return 1;
    }
    print_result(result);
    return 0;
}

std::optional<ml::Calculation> CalculatorApp::parse_json(const std::string& json_str) {
    std::optional<ml::Calculation::OperationType> op;
    std::optional<int64_t> left, right;
    try {
        auto j = json::parse(json_str);
        if (j.contains("left")) left = { j["left"].get<int64_t>() };
        if (j.contains("operation")) op = parse_operation(j["operation"].get<std::string>());
        if (j.contains("right")) right = { j["right"].get<int64_t>() };
    } catch (const std::exception&) {
        return std::nullopt;
    }
    if (!left || !op) return std::nullopt;
    if (op.value() == ml::Calculation::OperationType::Fac && right.has_value()) return std::nullopt;
    if (op.value() != ml::Calculation::OperationType::Fac && !right.has_value()) return std::nullopt;

    return { ml::Calculation(left.value(), op.value(), right.value_or(0)) };
}

std::optional<ml::Calculation::OperationType> CalculatorApp::parse_operation(const std::string& op_str) {
    if (op_str.length() == 1) {
        switch (op_str[0]) {
            case '+': return { ml::Calculation::OperationType::Add };
            case '-': return { ml::Calculation::OperationType::Sub };
            case '*': return { ml::Calculation::OperationType::Mul };
            case '/': return { ml::Calculation::OperationType::Div };
            case '^': return { ml::Calculation::OperationType::Exp };
            case '!': return { ml::Calculation::OperationType::Fac };
        }
    }
    return std::nullopt;
}

void CalculatorApp::print_help(const std::string& program_name) {
    std::cerr
        << "Usage: " << program_name << " '<json>'\n"
        << "JSON fields:"
        << "\n\t" << "left      : integer"
        << "\n\t" << "operation : one of + - * / ^ !"
        << "\n\t" << "right     : integer (not needed for factorial)\n"
        << std::endl;
}

void CalculatorApp::print_result(int64_t value) {
    std::cout << value << std::endl;
}

void CalculatorApp::print_error(const std::string& what) {
    std::cerr << what << std::endl;
}
