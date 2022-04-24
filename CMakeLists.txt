cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wpedantic -Wconversion -Werror" )
project(Test)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -g -O3")


include_directories(cores, utils, tests, components, assemblers)
file(GLOB SOURCES "tests/*.cpp" "cores/snes/*.cpp" "utils/*.cpp" "cores/nes/*.cpp" "utils/assemblers/*.cpp")
add_executable(tests ${SOURCES})
