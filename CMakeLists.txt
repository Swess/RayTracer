cmake_minimum_required(VERSION 3.13)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories("/usr/local/include")
include_directories("/opt/X11/include")
include_directories(.)

add_executable(RayTracer
        CImg.h
        Helpers.h
        main.cpp
        NeededMath.h)
