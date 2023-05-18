cmake_minimum_required(VERSION 3.10)
project(rvemu)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

set(srcs ./src/rvemu.c
         ./src/machine.c)
add_executable(${PROJECT_NAME} ${srcs})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)