# CMakeList.txt : CMake project for MyProject, include source and define
# project specific logic here.
#


include_directories(" ${CMAKE_SOURCE_DIR}/Common/include")

# Add source to this project's executable.
add_executable (MyProjectMain "Main.cpp" )

#if (CMAKE_VERSION VERSION_GREATER 3.12)
 # set_property(TARGET MyProjectMain PROPERTY CXX_STANDARD 20)
#endif()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


# Link Google Test to your test executable
target_link_libraries(MyProjectMain gtest gtest_main)

# TODO: Add tests and install targets if needed.
