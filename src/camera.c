#include "../src/ray.c"

// Structure representing a camera in 3D space
typedef struct {
    Vector position;     // Camera position in world space
    Vector direction;    // Forward direction of the camera
    Vector upVector;     // Upward direction of the camera
    Vector rightVector;  // Rightward direction of the camera (computed from up and direction)
    float fieldOfView;   // Field of view (FOV) in degrees
    float aspectRatio;   // Screen width / screen height
} Camera;

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