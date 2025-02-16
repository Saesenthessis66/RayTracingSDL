#include <SDL3/SDL.h>

#include <stdio.h>

// Structure representing a 3D vector
typedef struct{
    float x, y, z;
} Vector;

// Structure representing a ray with an origin and a direction
typedef struct {
    Vector origin; 
    Vector direction;
} Ray;

// Structure representing a camera in 3D space
typedef struct {
    Vector position;     // Camera position in world space
    Vector direction;    // Forward direction of the camera
    Vector upVector;     // Upward direction of the camera
    Vector rightVector;  // Rightward direction of the camera (computed from up and direction)
    float fieldOfView;   // Field of view (FOV) in degrees
    float aspectRatio;   // Screen width / screen height
} Camera;

// Function to compute the length (magnitude) of a vector
float vectorLength(Vector v)
{
    return SDL_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Function to multiply a vector by a scalar
Vector multiplyVector(Vector v, float multiplier)
{
    v.x *= multiplier;
    v.y *= multiplier;
    v.z *= multiplier;
    return v;
}

// Function to add two vectors
Vector addVectors(Vector u, Vector v)
{
    u.x += v.x;
    u.y += v.y;
    u.z += v.z;
    return u;
}

// Function to normalize a vector (make its length = 1)
Vector normalizeVector(Vector v) {
    float l = vectorLength(v);
    if (l == 0) { 
        printf("Cannot normalize a zero vector!\n");
        return v;  // Return unchanged vector if it's zero-length to avoid division by zero
    }
    Vector result = {v.x / l, v.y / l, v.z / l};
    return result;
}

// Function to compute the cross product of two vectors (useful for generating orthogonal directions)
Vector vectorCrossProduct(Vector u, Vector v)
{
    Vector cross;
    cross.x = u.y * v.z - u.z * v.y;
    cross.y = u.z * v.x - u.x * v.z;
    cross.z = u.x * v.y - u.y * v.x;
    return cross;
}

// Function to initialize the camera with position, direction, up vector, and FOV
void initCamera(Camera* camera, Vector pos, Vector dir, Vector up, float fov, int screenWidth, int screenHeight)
{
    camera->position = pos;
    camera->direction = normalizeVector(dir); // Ensure direction is normalized
    camera->upVector = normalizeVector(up);   // Ensure up vector is normalized
    camera->fieldOfView = fov;
    camera->aspectRatio = (float)screenWidth / (float)screenHeight; // Compute aspect ratio

    // Compute the right vector using the cross product of up and direction
    camera->rightVector = normalizeVector(vectorCrossProduct(camera->upVector, camera->direction));

    // Recompute the up vector to ensure it's truly perpendicular to both direction and right vectors
    camera->upVector = vectorCrossProduct(camera->rightVector, camera->direction);
}

// Function to generate a ray from a given pixel position on the screen
Ray mapPixelToRay(Camera* camera, int pixelX, int pixelY, int screenWidth, int screenHeight)
{
    float computedX, computedY, scale;
    
    // Convert pixel coordinates to normalized device coordinates (NDC) ranging from -1 to 1
    computedX = 2 * (pixelX + 0.5) / screenWidth - 1;
    computedY = 1 - 2 * (pixelY + 0.5) / screenHeight;

    // Compute the scaling factor using the tangent of half the FOV (converted to radians)
    scale = SDL_tan((camera->fieldOfView * SDL_PI_F / 180.0) / 2);
    computedX *= scale; // Scale the x coordinate by FOV
    computedY *= scale; // Scale the y coordinate by FOV
    computedX *= camera->aspectRatio; // Apply aspect ratio correction to x

    Ray ray;
    ray.origin = camera->position; // Ray starts from the camera position

    // Compute the direction of the ray by adding the weighted right and up vectors to the camera direction
    Vector dirSum = addVectors(
        multiplyVector(camera->rightVector, computedX),
        multiplyVector(camera->upVector, computedY)
    );

    ray.direction = addVectors(dirSum, camera->direction); // Combine with the forward direction
    ray.direction = normalizeVector(ray.direction); // Normalize to maintain consistent direction length
    
    return ray;
}