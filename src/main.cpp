#include <cinttypes>
#include <cstdio>
#include <getopt.h>
#include <math_lib/math_functions.hpp>

struct options {
    char *left, *operation, *right;
};

bool parse(int argc, char* argv[], options& opts) {
    static struct option long_options[] = {{"help", 0, 0, 0}, {0, 0, 0, 0}};
    int opt_index = 0;
    if (getopt_long(argc, argv, "", long_options, &opt_index) != -1)
        return false; // just print help
    int remaining_argc = argc - optind;
    if (remaining_argc == 1)
        return false;
    if (remaining_argc >= 2) {
        if (remaining_argc > 3)
            return false;
        opts.left = argv[optind++];
        opts.operation = argv[optind++];
        opts.right = (remaining_argc == 3) ? argv[optind++] : nullptr;
    }
    return true;
}

bool stoint64(const char* s, int64_t& result) {
    if (s == nullptr)
        return false;
    if (s[0] == '\0')
        return false;
    bool has_minus = (s[0] == '-');
    if (has_minus && s[1] == '\0')
        return false;
    if (s[has_minus] == '0')
        return false;
    result = 0;
    for (size_t i = has_minus; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9')
            return false;
        if ((has_minus && INT64_MIN / 10 > result) ||
            (INT64_MAX / 10 < result || INT64_MAX - (s[i] - '0') < 10 * result))
            return false;
        result = result * 10 + (s[i] - '0');
    }
    return true;
}

bool stoot(const char* s, ml::Calculation::OperationType& type) {
    if (s == nullptr || s[0] == '\0' || s[1] != '\0')
        return false;
    switch (s[0]) {
    case '+':
        type = ml::Calculation::OperationType::Add;
        break;
    case '-':
        type = ml::Calculation::OperationType::Sub;
        break;
    case '*':
        type = ml::Calculation::OperationType::Mul;
        break;
    case '/':
        type = ml::Calculation::OperationType::Div;
        break;
    case '^':
        type = ml::Calculation::OperationType::Exp;
        break;
    case '!':
        type = ml::Calculation::OperationType::Fac;
        break;
    default:
        return false;
    }
    return true;
}

bool check(const options& opts, ml::Calculation& calc) {
    if (!stoint64(opts.left, calc.left))
        return false;
    if (!stoot(opts.operation, calc.operation_type))
        return false;
    if (calc.operation_type == ml::Calculation::OperationType::Fac)
        return opts.right == nullptr;
    else
        return stoint64(opts.right, calc.right);
}

void print_help(const char* program) {
    fprintf(stderr,
            "Usage: %s -- <left_operand> [+-/*^] <right_operand> or "
            "%s -- <operand> !\n"
        "where operands are integers in range [-2^64 - 1; 2^64]\n",
        program, program);
}

void print_result(const ml::Calculation& calc) {
    if (calc.error == ml::Calculation::ErrorCode::NoError)
        fprintf(stdout, "%" PRId64 "\n", calc.result);
    else
        fprintf(stderr, "%s", ml::to_str(calc.error));
}

void run(int argc, char* argv[]) {
    options opts{};
    ml::Calculation calc;
    if (parse(argc, argv, opts) && check(opts, calc)) {
        ml::calculate(calc);
        print_result(calc);
    } else
        print_help(argv[0]);
}

int main(int argc, char* argv[]) {
    run(argc, argv);
    return 0;
}