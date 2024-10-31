cmake_minimum_required(VERSION 3.12)
project(c_data_structures)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

if(UNIX)
    # Enable profiling with gprof
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif(UNIX)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(CTest)
enable_testing()

add_subdirectory(utilities)
add_subdirectory(linked_list)
add_subdirectory(contiguous_array)