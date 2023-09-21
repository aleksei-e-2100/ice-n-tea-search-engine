cmake_minimum_required(VERSION 3.20)
project("iCE && tEA Search Engine" VERSION 1.0)

set(C_MAKE_CXX_STANDART 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tests)

include(FetchContent)

FetchContent_Declare(json 
                     GIT_REPOSITORY https://github.com/nlohmann/json.git 
                     GIT_TAG v3.11.2)
FetchContent_MakeAvailable(json)
