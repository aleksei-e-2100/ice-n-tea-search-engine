cmake_minimum_required(VERSION 3.20)

set(C_MAKE_CXX_STANDART 17)

set(PROJECT_SOURCES
    srchSrvTests.cpp
    ${INCLUDE_DIR}/invertedIndex.h 
    ${SRC_DIR}/invertedIndex.cpp
    ${INCLUDE_DIR}/searchServer.h 
    ${SRC_DIR}/searchServer.cpp
)

set(RUNTIME_OUTPUT_NAME SrchSrvTests)

enable_testing()

add_executable(${RUNTIME_OUTPUT_NAME} ${PROJECT_SOURCES})
target_include_directories(${RUNTIME_OUTPUT_NAME} PUBLIC ${INCLUDE_DIR} ${SRC_DIR})
target_link_libraries(${RUNTIME_OUTPUT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${RUNTIME_OUTPUT_NAME})
