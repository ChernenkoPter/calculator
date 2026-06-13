include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

if(BUILD_MATH_LIB_TESTS)
    file(GLOB LIB_TEST_FILES ${math_lib_SOURCE_DIR}/gtests/*.cpp)
    list(APPEND TEST_FILES ${LIB_TEST_FILES})
endif()

add_executable(${PROJECT_NAME}_tests ${TEST_FILES})

enable_testing()
target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest_main math_lib)
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME}_tests PRIVATE WITH_TESTS)
target_compile_features(${PROJECT_NAME}_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)