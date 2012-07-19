cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

# Necessary for c++11 usage in Helpers subsubmodule
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

include_directories(/media/portable/src/Eigen)

FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase
             ITKImageIntensity ITKImageFeature ITKMathematicalMorphology ITKBinaryMathematicalMorphology
             ITKDeprecated)
INCLUDE(${ITK_USE_FILE})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(PatchComparison_includes ${ITKHelpers_includes} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

# Submodules
if(NOT TARGET EigenHelpers)
  add_subdirectory(EigenHelpers)
endif()

if(NOT TARGET Mask)
  add_subdirectory(Mask)
  include_directories(${Mask_includes})
endif()

if(NOT TARGET PatchProjection)
  add_subdirectory(PatchProjection)
  include_directories(${PatchProjection_includes})
endif()

add_library(PatchComparison
PatchComparison.cpp
DiffusionDistance.cpp)
target_link_libraries(PatchComparison ${ITK_LIBRARIES} ITKHelpers)

SET(PatchComparison_BuildTests OFF CACHE BOOL "PatchComparison_BuildTests")
if(PatchComparison_BuildTests)
  add_subdirectory(Tests)
endif()
