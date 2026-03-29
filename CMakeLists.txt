cmake_minimum_required(VERSION 3.14)
project(calculator VERSION 0.1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

include(FetchContent)

FetchContent_Declare(
    math_lib
    GIT_REPOSITORY https://github.com/ChernenkoPter/math_lib.git
    GIT_TAG main
)

FetchContent_MakeAvailable(math_lib)

target_link_libraries(${PROJECT_NAME} PRIVATE math_lib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
