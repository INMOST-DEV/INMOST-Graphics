cmake_minimum_required(VERSION 2.8)
project(Graphics)

find_package(inmost REQUIRED)

link_directories(${INMOST_LIBRARY_DIRS})
include_directories(${INMOST_INCLUDE_DIRS})
add_definitions(${INMOST_DEFINITIONS})

add_subdirectory(DrawMatrix)
add_subdirectory(DrawGrid)
add_subdirectory(DrawGridTests)
add_subdirectory(Octree)
add_subdirectory(OctreeCutcell)
