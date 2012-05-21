cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
 
add_library(PatchComparison PatchComparison.cpp)
target_link_libraries(${ITK_LIBRARIES})

ADD_EXECUTABLE(Example Example.cpp)
TARGET_LINK_LIBRARIES(Example PatchComparison)
