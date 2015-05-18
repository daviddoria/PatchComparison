cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(CMakeHelpers)

# Enable c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11")

# Set the path to install executables to
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

# Tell CMake to also look in the source directory to find some .cmake files (Eigen3)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

# Set the CMake variable to the value of the environment variable.
# If the build is absolutely fresh, this will be the case if the CMake variable EIGEN3_INCLUDE_DIR is not set.
if(NOT EIGEN3_INCLUDE_DIR)
  set(EIGEN3_INCLUDE_DIR $ENV{EIGEN3_INCLUDE_DIR})
  message("Set EIGEN3_INCLUDE_DIR to ${EIGEN3_INCLUDE_DIR} from environment variable of the same name.")
else()
  message("EIGEN3_INCLUDE_DIR is ${EIGEN3_INCLUDE_DIR}")
endif()

if(NOT EIGEN3_FOUND)
  FIND_PACKAGE(Eigen3 REQUIRED) #requires FindEigen3.cmake to be in the source directory
  include_directories(${EIGEN3_INCLUDE_DIR})
  set(PatchComparison_includes ${PatchComparison_includes} ${EIGEN3_INCLUDE_DIR})
endif()

# ITK
if(NOT ITK_FOUND)
  FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase ITKTestKernel ITKIOPNG ITKIOMeta
              ITKImageIntensity ITKImageFeature ITKMathematicalMorphology
              ITKBinaryMathematicalMorphology ITKDistanceMap)
  INCLUDE(${ITK_USE_FILE})
endif()

# Boost
FIND_PACKAGE(Boost COMPONENTS regex) # need the libboost-regex1.48.0 and libboost-regex1.48.0-dev packages installed (Ubuntu). Without the -dev package, CMake says "Could NOT find Boost."
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

# Submodules
UseSubmodule(Histogram PatchComparison)
UseSubmodule(Mask PatchComparison)
UseSubmodule(PatchProjection PatchComparison)

CreateSubmodule(PatchComparison)

add_library(PatchComparison
PatchComparison.cpp
DiffusionDistance.cpp
PairReader.cpp)
target_link_libraries(PatchComparison ${PatchComparison_libraries} boost_regex)

SET(PatchComparison_BuildTests OFF CACHE BOOL "PatchComparison_BuildTests")
if(PatchComparison_BuildTests)
  add_subdirectory(Tests)
endif()

SET(PatchComparison_BuildDrivers OFF CACHE BOOL "PatchComparison_BuildDrivers")
if(PatchComparison_BuildDrivers)
  add_subdirectory(Drivers)
endif()

#GET_DIRECTORY_PROPERTY(AllPatchComparisonIncludes INCLUDE_DIRECTORIES)
#message("Final PatchComparison includes: ${AllPatchComparisonIncludes}")
