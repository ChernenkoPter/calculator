#include <cstdio>
#include <unordered_set>
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
    utils::Logger::instance().debug("Starting calculation");
    try {
        result = calc.value().calculate();
    } catch (const CalculationError& e) {
        utils::Logger::instance().error(std::string("Calculation failed: ") + e.what());
        return 1;
    }
    utils::Logger::instance().debug("Result calculated successfully");
    print_result(result);
    return 0;
}

std::optional<ml::Calculation> CalculatorApp::parse_json(const std::string& json_str) {
    utils::Logger::instance().debug(std::string("Parsing JSON: ") + json_str);
    std::optional<ml::Calculation::OperationType> op;
    std::optional<int64_t> left, right;
    std::unordered_set<std::string> keys;
    try {
        auto j = json::parse(json_str);
        if (j.contains("left")) left = { j["left"].get<int64_t>() };
        if (j.contains("operation")) op = parse_operation(j["operation"].get<std::string>());
        if (j.contains("right")) right = { j["right"].get<int64_t>() };
            
    } catch (const std::exception& e) {
        utils::Logger::instance().error(std::string("JSON parsing failed") + e.what());
        return std::nullopt;
    }
    utils::Logger::instance().debug("JSON parsed successfully");

    std::unordered_set<std::string> allowed = {"left", "operation", "right"};
    std::unordered_set<std::string> unknown;
    for (const auto& key : keys) {
        if (!allowed.count(key)) {
            unknown.emplace(key);
        }
    }
    if (!unknown.empty()) {
        std::string unknown_keys;
        for (const auto& k : unknown) {
            if (!unknown_keys.empty()) unknown_keys += ", ";
            unknown_keys += k;
        }
        utils::Logger::instance().warn(std::string("Unknown keys: ") + unknown_keys);
    }

    bool invalid_json = false;
    if (!left) { 
        utils::Logger::instance().error("There is no \"left\" key in JSON");
        invalid_json = true;
    }
    if (!op) {
        utils::Logger::instance().error("There is no \"operation\" key in JSON");
        invalid_json = true;
        if (!right) utils::Logger::instance().warn("There is no \"right\" key in JSON");
    } else if (op.value() == ml::Calculation::OperationType::Fac && right.has_value())
        utils::Logger::instance().warn("JSON contains unary operation and right operand");
    if (op && op.value() != ml::Calculation::OperationType::Fac && !right.has_value()) {
        utils::Logger::instance().error("There is no \"right\" key in JSON with binary operation");
        invalid_json = true;
    }
    
    if (invalid_json) return std::nullopt;

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