cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wpedantic -Werror" )
project(Test)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -g")


include_directories(core, utils, tests)
file(GLOB SOURCES "tests/*.cpp" "core/*.cpp" "utils/*.cpp")
add_executable(tests ${SOURCES})
