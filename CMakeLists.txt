cmake_minimum_required(VERSION 3.14)
project(calculator VERSION 0.1.0 LANGUAGES CXX)

add_executable(calculator src/main.cpp)

include(FetchContent)

FetchContent_Declare(
    math_lib
    GIT_REPOSITORY https://github.com/ChernenkoPter/math_lib.git
    GIT_TAG main
)

FetchContent_MakeAvailable(math_lib)

target_link_libraries(calculator PRIVATE math_lib)

target_compile_features(calculator PRIVATE cxx_std_17)
