cmake_minimum_required(VERSION 3.13)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)


SET(GCC_COVERAGE_COMPILE_FLAGS "-lX11 -lpthread")
# SET(GCC_COVERAGE_LINK_FLAGS    "-lX11 -lpthread")

include_directories(/usr/local/include)
include_directories(/opt/X11/include)
include_directories(glm)
include_directories(.)

link_directories(/opt/X11/lib)


# add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(RayTracer
        geometry.h
        main.cpp
        NeededMath.h)
