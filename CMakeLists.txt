cmake_minimum_required(VERSION 3.16)
project(Test)

set(CMAKE_CXX_STANDARD 17)

include_directories(core, utils, tests)
file(GLOB SOURCES "tests/*.cpp" "core/*.cpp" "utils/*.cpp")
add_executable(tests ${SOURCES})
