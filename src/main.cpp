#include <cstdint>
#include <cassert>
#include <math_lib/math_functions.hpp>

int main() {
    {
        ml::Calculation a = {5, 0, .operation_type = ml::Calculation::OperationType::Add};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 5);
    }

    {
        ml::Calculation a = {5, 0, .operation_type = ml::Calculation::OperationType::Sub};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 5);
    }

    {
        ml::Calculation a = {5, 0, .operation_type = ml::Calculation::OperationType::Mul};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 0);
    }

    {
        ml::Calculation a = {5, 0, .operation_type = ml::Calculation::OperationType::Div};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::DivByZero);
    }

    {
        ml::Calculation a = {5, 12, .operation_type = ml::Calculation::OperationType::Div};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 0);
    }

    {
        ml::Calculation a = {5, .operation_type = ml::Calculation::OperationType::Fac};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 120);
    }

    {
        ml::Calculation a = {50, .operation_type = ml::Calculation::OperationType::Fac};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::Overflow);
    }

    {
        ml::Calculation a = {-1, .operation_type = ml::Calculation::OperationType::Fac};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NegativeFactorial);
    }

    {
        ml::Calculation a = {5, 3, .operation_type = ml::Calculation::OperationType::Exp};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NoError);
        assert(a.result == 125);
    }

    {
        ml::Calculation a = {5, 123, .operation_type = ml::Calculation::OperationType::Exp};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::Overflow);
    }

    {
        ml::Calculation a = {5, -2, .operation_type = ml::Calculation::OperationType::Exp};
        ml::calculate(a);
        assert(a.error == ml::Calculation::ErrorCode::NegativePower);
    }
    return 0;
}