cmake_minimum_required(VERSION 3.20)

set(C_MAKE_CXX_STANDART 17)

set(PROJECT_SOURCES
    indxTests.cpp
    ${INCLUDE_DIR}/invertedIndex.h 
    ${SRC_DIR}/invertedIndex.cpp
)

set(RUNTIME_OUTPUT_NAME IndxTests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable(${RUNTIME_OUTPUT_NAME} ${PROJECT_SOURCES})
target_include_directories(${RUNTIME_OUTPUT_NAME} PUBLIC ${INCLUDE_DIR} ${SRC_DIR})
target_link_libraries(${RUNTIME_OUTPUT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${RUNTIME_OUTPUT_NAME})
