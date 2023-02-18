cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

execute_process(COMMAND conan install .. --build=missing)
set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(${CMAKE_CURRENT_BINARY_DIR}/conan_paths.cmake)

add_compile_options(-fsanitize=address -fsanitize=return -fsanitize=null)
#  -fsanitize=address grpc client will report error
add_link_options(-fsanitize=leak -fsanitize=address )

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(MY_LIB mylib)
set(MY_PROTO myproto)

add_subdirectory(./proto)

add_subdirectory(./src)
