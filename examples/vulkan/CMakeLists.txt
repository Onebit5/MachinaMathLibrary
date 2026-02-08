cmake_minimum_required(VERSION 3.16)
project(MMLVulkanExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FetchContent_MakeAvailable(glfw)

add_executable(01_spheres 01_spheres.cpp)
target_include_directories(01_spheres PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(01_spheres PRIVATE Vulkan::Vulkan glfw)

add_executable(02_quaternion_slerp 02_quaternion_slerp.cpp)
target_include_directories(02_quaternion_slerp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(02_quaternion_slerp PRIVATE Vulkan::Vulkan glfw)

add_executable(03_transforms 03_transforms.cpp)
target_include_directories(03_transforms PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(03_transforms PRIVATE Vulkan::Vulkan glfw)

add_executable(04_free_camera 04_free_camera.cpp)
target_include_directories(04_free_camera PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(04_free_camera PRIVATE Vulkan::Vulkan glfw)

message(STATUS "Vulkan examples configured")
message(STATUS "Vulkan SDK: ${Vulkan_INCLUDE_DIR}")
