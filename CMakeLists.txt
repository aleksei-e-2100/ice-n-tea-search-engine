cmake_minimum_required(VERSION 3.20)
project("iCE && tEA Search Engine" VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/search_engine/include")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/search_engine/src")

add_subdirectory(search_engine)
add_subdirectory(tests)

include(FetchContent)

# Add JSON for Modern C++
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0)
FetchContent_MakeAvailable(json)

# Add GoogleTest
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0)

if(MSVC)
    set(gtest_disable_pthreads on)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
