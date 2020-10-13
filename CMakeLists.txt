cmake_minimum_required(VERSION 3.11)
project(xt-test)

include(FetchContent)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(XTENSOR_USE_XSIMD 1 CACHE INTERNAL "")

macro(find_fetch package repo)
    find_package(${package} QUIET)
    if(NOT ${package}_FOUND)
        message(STATUS "fetching ${package} from ${repo}")
        FetchContent_Declare(${package}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(${package})
        list(APPEND CMAKE_PREFIX_PATH "${${package}_BINARY_DIR}")
    endif()
endmacro()

find_fetch(fp16 https://github.com/0xBYTESHIFT/fp16.git)
FetchContent_MakeAvailable(fp16)
list(APPEND CMAKE_PREFIX_PATH "${fp16_BINARY_DIR}")
find_fetch(xtl https://github.com/xtensor-stack/xtl.git)
find_fetch(xtensor https://github.com/xtensor-stack/xtensor.git)
find_fetch(xsimd https://github.com/xtensor-stack/xsimd.git)

add_dependencies(xtensor xtl xsimd)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} \
    -O3 -s -ffast-math -march=native\
    -DXTENSOR_USE_XSIMD=1 \
    -DXTENSOR_DISABLE_EXCEPTIONS=1 \
    -DNDEBUG")

# add_executable(test_padding_speed test_padding_speed.cpp)
# target_link_libraries(test_padding_speed xtensor xtl half)
# add_test(test_padding_speed test_padding_speed)

add_executable(test_half_integration test_half_integration.cpp)
target_link_libraries(test_half_integration xtensor xtl half)
add_test(test_half_integration test_half_integration)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
