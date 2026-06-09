#include <calculator_app.hpp>
#include <logger.hpp>

int main(int argc, char* argv[]) {
    utils::Logger::instance().init();
    CalculatorApp app;
    return app.run(argc, argv);
}