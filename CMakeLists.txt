cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(/media/portable/src/Eigen)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

# Submodules
add_subdirectory(EigenHelpers)
add_subdirectory(Mask)

add_library(PatchComparison
PatchComparison.cpp
DiffusionDistance.cpp
SSD.cpp)
target_link_libraries(PatchComparison ${ITK_LIBRARIES} ITKHelpers)

add_executable(PotentialBadMatches PotentialBadMatches.cpp)
target_link_libraries(PotentialBadMatches PatchComparison)
