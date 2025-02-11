cmake_minimum_required(VERSION 3.16)
project(RayTracingSDL C)

# Find SDL3
find_package(SDL3 REQUIRED)

# Add executable
add_executable(RayTracingSDL src/main.c)

# Link SDL3
target_link_libraries(RayTracingSDL PRIVATE SDL3::SDL3)

# Set C standard
set_target_properties(RayTracingSDL PROPERTIES C_STANDARD 11)
