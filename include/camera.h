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

// Function to move the camera forward along the direction vector
void moveCameraForward(Camera* camera);

// Function to move the camera backward along the direction vector
void moveCameraBackward(Camera* camera);

// Function to move the camera left along the right vector
void moveCameraLeft(Camera* camera);

// Function to move the camera right along the right vector
void moveCameraRight(Camera* camera);

// Function to move the camera up along the up vector
void moveCameraUp(Camera* camera);

// Function to move the camera down along the up vector
void moveCameraDown(Camera* camera);

// Function to rotate the camera up around the right vector
void rotateCameraUp(Camera* camera);

// Function to rotate the camera down around the right vector
void rotateCameraDown(Camera* camera);

// Function to rotate the camera left around the up vector
void rotateCameraLeft(Camera* camera);

// Function to rotate the camera right around the up vector
void rotateCameraRight(Camera* camera);

#endif // CAMERA_H