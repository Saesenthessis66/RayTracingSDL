#ifndef RENDER_FUNCTIONS_H
#define RENDER_FUNCTIONS_H

#include "camera.h"
#include "illumination.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

typedef struct {
    Vector point; // The intersection point
    Vector normal; // The normal at the intersection point
    Material material; // The material of the intersected object
    int objectType; // The type of the intersected object
} ObjectIntersection;

// Initializes the scene with default objects and lighting.
void initialize_scene(int WINDOW_WIDTH, int WINDOW_HEIGHT, Camera* camera, Scene* scene);

// Computes the color of a pixel by tracing a ray from the camera through the scene.
SDL_Color computePixelColor(int WINDOW_WIDTH, int WINDOW_HEIGHT, int x, int y, Camera* camera, Scene* scene);

// Finds the closest intersection of a ray with objects in the scene.
ObjectIntersection getClosestObjectIntersection(Ray ray, Scene* scene);

#endif // RENDER_FUNCTIONS_H