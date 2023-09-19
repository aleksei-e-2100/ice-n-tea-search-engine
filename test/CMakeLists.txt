cmake_minimum_required(VERSION 3.20)

set(C_MAKE_CXX_STANDART 17)

set(PROJECT_SOURCES
    projectTests.cpp
)

set(RUNTIME_OUTPUT_NAME ProjectTest)

include(FetchContent)
FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG adc514538678a61b13c240f7b41babbc03b2ac24)
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${RUNTIME_OUTPUT_NAME} ${PROJECT_SOURCES})
target_include_directories(${RUNTIME_OUTPUT_NAME} PUBLIC ${INCLUDE_DIR} ${SRC_DIR})
target_link_libraries(${RUNTIME_OUTPUT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${RUNTIME_OUTPUT_NAME})
