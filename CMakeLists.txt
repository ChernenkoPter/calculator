cmake_minimum_required(VERSION 3.14)
project(calculator VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    math_lib
    GIT_REPOSITORY https://github.com/ChernenkoPter/math_lib.git
    GIT_TAG second_hometask
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(math_lib spdlog json)

file(GLOB_RECURSE CALC_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${CALC_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE math_lib spdlog::spdlog nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|GNU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
    )
endif()

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
