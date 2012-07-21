cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

# Necessary for c++11 usage in Helpers subsubmodule
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

# Set the path to install executables to
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

# Eigen
include_directories(/media/portable/src/Eigen)

# ITK
FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase ITKTestKernel
             ITKImageIntensity ITKImageFeature ITKMathematicalMorphology ITKBinaryMathematicalMorphology ITKDistanceMap)
INCLUDE(${ITK_USE_FILE})

# Boost
FIND_PACKAGE(Boost COMPONENTS regex) # need the libboost-regex1.48.0 and libboost-regex1.48.0-dev packages installed (Ubuntu). Without the -dev package, CMake says "Could NOT find Boost."
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(PatchComparison_includes ${ITKHelpers_includes} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

# Submodules
if(NOT TARGET EigenHelpers)
  add_subdirectory(EigenHelpers)
  include_directories(${EigenHelpers_includes})
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
DiffusionDistance.cpp
PairReader.cpp)
target_link_libraries(PatchComparison ${ITK_LIBRARIES} ITKHelpers boost_regex)

SET(PatchComparison_BuildTests OFF CACHE BOOL "PatchComparison_BuildTests")
if(PatchComparison_BuildTests)
  add_subdirectory(Tests)
endif()

add_subdirectory(Drivers)
