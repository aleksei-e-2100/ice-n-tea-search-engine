cmake_minimum_required(VERSION 3.20)

set(C_MAKE_CXX_STANDART 17)

include(FetchContent)
FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG adc514538678a61b13c240f7b41babbc03b2ac24)
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(indxTests)
add_subdirectory(srchSrvTests)
