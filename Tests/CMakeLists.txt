# Allow headers in tests to be included like
# #include "PatchComparison.h" rather than needing
# #include "PatchComparison/PatchComparison.h"
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(TestPatchComparison TestPatchComparison.cpp)
target_link_libraries(TestPatchComparison PatchComparison)

add_executable(TestPixelDifferences TestPixelDifferences.cpp)
target_link_libraries(TestPixelDifferences PatchComparison)

add_executable(TestSelfPatchCompare TestSelfPatchCompare.cpp)
target_link_libraries(TestSelfPatchCompare PatchComparison Mask)

add_executable(TestSelfPatchCompareLocalOptimization TestSelfPatchCompareLocalOptimization.cpp)
target_link_libraries(TestSelfPatchCompareLocalOptimization PatchComparison Mask)
