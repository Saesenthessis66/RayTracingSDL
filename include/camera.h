#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"  // Include the ray header file to use the Ray structure

// Structure representing a camera in 3D space
typedef struct {
    Vector position;     // Camera position in world space
    Vector direction;    // Forward direction of the camera
    Vector upVector;     // Upward direction of the camera
    Vector rightVector;  // Rightward direction of the camera (computed from up and direction)
    float fieldOfView;   // Field of view (FOV) in degrees
    float aspectRatio;   // Screen width / screen height
} Camera;

// Function to initialize a camera with position, direction, up vector, FOV, and screen dimensions
void initCamera(Camera* camera, Vector pos, Vector dir, Vector up, float fov, int screenWidth, int screenHeight);

// Function to generate a ray from a given pixel position on the screen
Ray mapPixelToRay(Camera* camera, int pixelX, int pixelY, int screenWidth, int screenHeight);

#endif // CAMERA_H