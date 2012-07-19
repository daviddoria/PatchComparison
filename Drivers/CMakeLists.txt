# So that things like #include "SSD.h" can be used directly in this subdirectory (instead of needing #include "../SSD.h")
include_directories(../)

add_executable(Example Example.cpp)

add_executable(CompareOverBlur CompareOverBlur.cpp)
target_link_libraries(CompareOverBlur PatchComparison)
