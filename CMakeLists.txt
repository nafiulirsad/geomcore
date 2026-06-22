cmake_minimum_required(VERSION 3.16)

project(GeomCore
    VERSION 0.1.0
    DESCRIPTION "High-performance C++17 BIM geometry and topology engine"
    LANGUAGES CXX)

# ---- Language standard -------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- Library -----------------------------------------------------------------
add_library(geomcore
    src/vec.cpp
    src/mat.cpp
    src/tolerance.cpp
    src/allocator.cpp
    src/topology.cpp
    src/invariants.cpp)

target_include_directories(geomcore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_features(geomcore PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(geomcore PRIVATE /W4)
else()
    target_compile_options(geomcore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- Test executable ---------------------------------------------------------
enable_testing()

add_executable(geomcore_tests tests/test_main.cpp)
target_link_libraries(geomcore_tests PRIVATE geomcore)

add_test(NAME geomcore_smoke COMMAND geomcore_tests)
