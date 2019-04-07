cmake_minimum_required(VERSION 3.13)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp CImg.h NeededMath.h Helpers.h)