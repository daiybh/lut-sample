cmake_minimum_required(VERSION 3.10)
 project(MyProject VERSION 0.1.0 LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(CUDAToolkit)

file(GLOB source_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu")
add_executable(MyExecutable ${source_cpp})
target_include_directories(MyExecutable PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/eigen3/")

set(INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${INCLUDE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})

    # Silence irrelevant Eigen3 warnings
    if (MSVC)
        set(MSVC_UNKNOWN_PRAGMA "-Xcompiler" "/wd4068")
    endif()

    target_compile_options(MyExecutable PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
        --diag-suppress 20012,20014 
        --expt-relaxed-constexpr 
        ${MSVC_UNKNOWN_PRAGMA}>
    )
