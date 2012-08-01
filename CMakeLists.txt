cmake_minimum_required(VERSION 2.6)

Project(PatchComparison)

# Enable c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

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

FIND_PACKAGE(Eigen3 REQUIRED) #requires FindEigen3.cmake to be in the source directory
include_directories(${EIGEN3_INCLUDE_DIR})


# ITK
FIND_PACKAGE(ITK REQUIRED ITKCommon ITKIOImageBase ITKTestKernel ITKIOPNG ITKIOMeta
             ITKImageIntensity ITKImageFeature ITKMathematicalMorphology
             ITKBinaryMathematicalMorphology ITKDistanceMap)
INCLUDE(${ITK_USE_FILE})

# Boost
FIND_PACKAGE(Boost COMPONENTS regex) # need the libboost-regex1.48.0 and libboost-regex1.48.0-dev packages installed (Ubuntu). Without the -dev package, CMake says "Could NOT find Boost."
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

# Submodules
if(NOT TARGET EigenHelpers)
  add_subdirectory(EigenHelpers)
  include_directories(${EigenHelpers_includes})
  set(PatchComparison_includes ${PatchComparison_includes} ${EigenHelpers_includes})
  set(PatchComparison_libraries ${PatchComparison_libraries} ${EigenHelpers_libraries})
endif()

if(NOT TARGET Histogram)
  add_subdirectory(Histogram)
  include_directories(${Histogram_includes})
  set(PatchComparison_includes ${PatchComparison_includes} ${Histogram_includes})
  #set(PatchComparison_libraries ${PatchComparison_libraries} ${Histogram_libraries}) # this is a header-only library
endif()

if(NOT TARGET Mask)
  add_subdirectory(Mask)
  include_directories(${Mask_includes})
  set(PatchComparison_includes ${PatchComparison_includes} ${Mask_includes})
  set(PatchComparison_libraries ${PatchComparison_libraries} ${Mask_libraries})
endif()

if(NOT TARGET PatchProjection)
  add_subdirectory(PatchProjection)
  include_directories(${PatchProjection_includes})
  set(PatchComparison_includes ${PatchComparison_includes} ${PatchProjection_includes})
  #set(PatchComparison_libraries ${PatchComparison_libraries} PatchProjection) # this is a header-only library
endif()

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(PatchComparison_includes ${EIGEN3_INCLUDE_DIR} ${PatchComparison_includes}
      ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
  set(PatchComparison_libraries ${PatchComparison_libraries} PatchComparison PARENT_SCOPE)
  message("PatchComparison submodule used from ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

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

GET_DIRECTORY_PROPERTY(AllPatchComparisonIncludes INCLUDE_DIRECTORIES)
message("Final PatchComparison includes: ${AllPatchComparisonIncludes}")
