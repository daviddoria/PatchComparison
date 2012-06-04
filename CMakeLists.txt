cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

# Necessary for c++11 usage in Helpers subsubmodule
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(/media/portable/src/Eigen)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

# Submodules
if(NOT TARGET EigenHelpers)
  add_subdirectory(EigenHelpers)
endif()

if(NOT TARGET Mask)
  add_subdirectory(Mask)
endif()

add_library(PatchComparison
PatchComparison.cpp
DiffusionDistance.cpp
SSD.cpp)
target_link_libraries(PatchComparison ${ITK_LIBRARIES} ITKHelpers)
