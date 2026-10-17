cmake_minimum_required(VERSION 3.20)

file(GLOB HEADERS ${INC_DIR}/*.h)
file(GLOB SOURCES ${SRC_DIR}/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/release")
set(RUNTIME_OUTPUT_NAME SearchEngine)

add_executable(${RUNTIME_OUTPUT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(${RUNTIME_OUTPUT_NAME}
    PRIVATE
        ${INC_DIR}
)

target_link_libraries(${RUNTIME_OUTPUT_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
)
