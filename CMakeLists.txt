cmake_minimum_required(VERSION 3.26)
project(MML VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(MML_USE_SIMD "Enable SIMD optimizations" ON)
option(MML_BUILD_TESTS "Build test suite" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT DEFINED PLATFORM)
    if(WIN32)
        set(PLATFORM "windows")
    elseif(UNIX AND NOT APPLE)
        set(PLATFORM "linux")
    else()
        message(FATAL_ERROR "Unsupported platform. Specify -DPLATFORM=windows or -DPLATFORM=linux")
    endif()
endif()

message(STATUS "Building for platform: ${PLATFORM}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE MML_HEADERS
    src/*.h
    src/*.hpp
)

add_library(mml INTERFACE)
target_sources(mml INTERFACE
    ${MML_HEADERS}
)
target_include_directories(mml INTERFACE
    ${CMAKE_SOURCE_DIR}/src
)

file(GLOB_RECURSE TEST_SOURCES
    tests/main.cpp
)

if(MML_BUILD_TESTS)
    add_executable(MML_tests ${TEST_SOURCES})
    target_include_directories(MML_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    )

    if(MML_USE_SIMD)
        if(MSVC)
            target_compile_options(MML_tests PRIVATE /arch:SSE2)
        endif()
        target_compile_definitions(MML_tests PRIVATE MML_USE_SIMD=1)
    else()
        target_compile_definitions(MML_tests PRIVATE MML_USE_SIMD=0)
    endif()

    if(PLATFORM STREQUAL "windows")
        target_link_libraries(MML_tests PRIVATE
            user32 gdi32 dwmapi
        )
        target_compile_definitions(MML_tests PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
            UNICODE
            _UNICODE
        )
    elseif(PLATFORM STREQUAL "linux")
        target_link_libraries(MML_tests PRIVATE X11)
    endif()

    if(MSVC)
        target_compile_options(MML_tests PRIVATE /W4)
    else()
        target_compile_options(MML_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_custom_target(run_tests
        COMMAND ${CMAKE_COMMAND} -E echo "Running MML tests..."
        COMMAND $<TARGET_FILE:MML_tests>
        DEPENDS MML_tests
    )
endif()

find_package(Python COMPONENTS Interpreter)
if(Python_Interpreter_FOUND)
    add_custom_target(generate_header
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/generate_header.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating single-header mml.hpp"
        VERBATIM
    )
endif()

if(MML_USE_SIMD)
    message(STATUS "SIMD optimizations enabled")
else()
    message(STATUS "SIMD optimizations disabled")
endif()

message(STATUS "Machina Math Library configuration complete")
