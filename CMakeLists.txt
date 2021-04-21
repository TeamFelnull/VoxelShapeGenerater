cmake_minimum_required(VERSION 3.19)
project(VoxelShapeGenerater)

set(CMAKE_CXX_STANDARD 14)

add_executable(VoxelShapeGenerater main.cpp shape.h json.hpp)