#ifndef RENDER_FUNCTIONS_H
#define RENDER_FUNCTIONS_H

#include <stdlib.h>
#include <SDL3/SDL.h>
#include "camera.h"
#include "illumination.h"
#include "scene.h"  
#include "bvh.h" 


// Initializes the scene with default objects and lighting.
void initialize_scene(int WINDOW_WIDTH, int WINDOW_HEIGHT, Camera* camera, Scene* scene);

// Computes the color of a pixel by tracing a ray from the camera through the scene.
SDL_Color computePixelColor(int WINDOW_WIDTH, int WINDOW_HEIGHT, int x, int y, Camera* camera, Scene* scene);

#endif // RENDER_FUNCTIONS_H