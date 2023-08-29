# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (MyProject)


# Include sub-projects.
add_subdirectory ("MyProjectMain")
add_subdirectory("Common")
add_subdirectory("CommonUnitTests")