cmake_minimum_required(VERSION 3.20)

set(C_MAKE_CXX_STANDART 17)

set(PROJECT_SOURCES
    main.cpp
    ${INCLUDE_DIR}/converterJSON.h
    converterJSON_1.cpp
    converterJSON_2.cpp
)

set(RUNTIME_OUTPUT_NAME SearchEngine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/release)

add_executable(${RUNTIME_OUTPUT_NAME} ${PROJECT_SOURCES})

target_include_directories(${RUNTIME_OUTPUT_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${RUNTIME_OUTPUT_NAME} PRIVATE nlohmann_json::nlohmann_json)