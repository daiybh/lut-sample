cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)


file(GLOB source_cpp "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
message(STATUS "source_cpp: ${source_cpp}")
add_executable(MyExecutable ${source_cpp})
target_include_directories(MyExecutable PUBLIC "./"
"C:\\Users\\daiyb\\.conan2\\p\\eigen3d88c0279cc26\\p\\include\\eigen3\\")
