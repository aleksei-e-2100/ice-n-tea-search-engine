cmake_minimum_required(VERSION 3.20)

set(RUNTIME_OUTPUT_NAME IndexTests)

enable_testing()

add_executable(${RUNTIME_OUTPUT_NAME}
    index_tests.cpp
    ${INC_DIR}/inverted_index.h
    ${SRC_DIR}/inverted_index.cpp
    ${INC_DIR}/thread_pool.h
    ${SRC_DIR}/thread_pool.cpp
)

target_include_directories(${RUNTIME_OUTPUT_NAME} PUBLIC ${INC_DIR})

target_link_libraries(${RUNTIME_OUTPUT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${RUNTIME_OUTPUT_NAME})
