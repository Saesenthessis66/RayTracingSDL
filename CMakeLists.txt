cmake_minimum_required(VERSION 3.16)
project(RayTracingSDL C)

# Find SDL3
find_package(SDL3 REQUIRED)

# Link files
add_executable(RayTracingSDL src/main.c
                             src/ray.c
                             src/ray.h
                             src/camera.c
                             src/camera.h
                             src/shapes.c
                             src/shapes.h
                             src/light_sources.c
                             src/light_sources.h
                             src/scene.c
                             src/scene.h)

# Link SDL3
target_link_libraries(RayTracingSDL PRIVATE SDL3::SDL3)

# Set C standard
set_target_properties(RayTracingSDL PROPERTIES C_STANDARD 11)