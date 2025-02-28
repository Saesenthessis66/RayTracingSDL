cmake_minimum_required(VERSION 3.16)
project(RayTracingSDL C)

include(FetchContent)  # Include FetchContent for dependency management

# Find SDL3
find_package(SDL3 REQUIRED)

# Add include directory
include_directories(include)

# Define the main executable
add_executable(RayTracingSDL 
    src/main.c
    src/ray.c
    src/camera.c
    src/shapes.c
    src/light_sources.c
    src/scene.c
    src/illumination.c
)

# Link SDL3
target_link_libraries(RayTracingSDL PRIVATE SDL3::SDL3)

# Set C standard
set_target_properties(RayTracingSDL PROPERTIES C_STANDARD 11)

# Fetch Unity testing framework (version 2.6.1)
FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.1  # Specify the exact version tag
)
FetchContent_MakeAvailable(Unity)

# Create test executable
add_executable(run_tests 
    tests/test_main.c   # Single main file
    tests/test_ray.c 
    tests/test_camera.c
    tests/test_shapes.c
    tests/test_lights.c
    tests/test_scene.c
    src/ray.c 
    src/camera.c
    src/shapes.c
    src/light_sources.c
    src/scene.c
    ${unity_SOURCE_DIR}/src/unity.c
)

# Ensure test executable includes the headers
target_include_directories(run_tests PRIVATE include)

# Link the test executable properly
target_link_libraries(run_tests PRIVATE unity SDL3::SDL3)

# Include Unity headers
target_include_directories(run_tests PRIVATE ${unity_SOURCE_DIR}/src)

# No need to link against `Unity` since it's compiled directly