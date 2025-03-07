#include "camera.h"

// Initializes the camera with a specific position, direction, and field of view.
void initCamera(Camera* camera, Vector pos, Vector dir, Vector up, float fov, int screenWidth, int screenHeight)
{
    camera->position = pos;
    camera->direction = normalizeVector(dir); // Ensure direction is normalized
    camera->upVector = normalizeVector(up);   // Ensure up vector is normalized

    camera->fieldOfView = fov;
    camera->aspectRatio = (float)screenWidth / (float)screenHeight; // Compute aspect ratio

    // Compute the right vector using the cross product of up and direction
    camera->rightVector = normalizeVector(vectorCrossProduct(camera->direction, camera->upVector));

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

void moveCameraForward(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->direction, 0.1f));    
}

void moveCameraBackward(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->direction, -0.1f));    
}

void moveCameraLeft(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->rightVector, -0.1f));    
}

void moveCameraRight(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->rightVector, 0.1f));
}

void moveCameraUp(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->upVector, 0.1f));
}

void moveCameraDown(Camera *camera)
{
    camera->position = addVectors(camera->position, multiplyVector( camera->upVector, -0.1f));
}

typedef struct Matrix4x4
{
    float m[4][4];
} Matrix4x4;

// Helper function to create a rotation matrix around an arbitrary axis
Matrix4x4 createRotationMatrix(Vector axis, float angle)
{
    float cosTheta = SDL_cosf(angle);
    float sinTheta = SDL_sinf(angle);
    float oneMinusCosTheta = 1.0f - cosTheta;

    axis = normalizeVector(axis);

    Matrix4x4 rotationMatrix;
    rotationMatrix.m[0][0] = cosTheta + axis.x * axis.x * oneMinusCosTheta;
    rotationMatrix.m[0][1] = axis.x * axis.y * oneMinusCosTheta - axis.z * sinTheta;
    rotationMatrix.m[0][2] = axis.x * axis.z * oneMinusCosTheta + axis.y * sinTheta;
    rotationMatrix.m[0][3] = 0.0f;

    rotationMatrix.m[1][0] = axis.y * axis.x * oneMinusCosTheta + axis.z * sinTheta;
    rotationMatrix.m[1][1] = cosTheta + axis.y * axis.y * oneMinusCosTheta;
    rotationMatrix.m[1][2] = axis.y * axis.z * oneMinusCosTheta - axis.x * sinTheta;
    rotationMatrix.m[1][3] = 0.0f;

    rotationMatrix.m[2][0] = axis.z * axis.x * oneMinusCosTheta - axis.y * sinTheta;
    rotationMatrix.m[2][1] = axis.z * axis.y * oneMinusCosTheta + axis.x * sinTheta;
    rotationMatrix.m[2][2] = cosTheta + axis.z * axis.z * oneMinusCosTheta;
    rotationMatrix.m[2][3] = 0.0f;

    rotationMatrix.m[3][0] = 0.0f;
    rotationMatrix.m[3][1] = 0.0f;
    rotationMatrix.m[3][2] = 0.0f;
    rotationMatrix.m[3][3] = 1.0f;

    return rotationMatrix;
}

// Helper function to apply a rotation matrix to a vector
Vector applyMatrixToVector(Matrix4x4 matrix, Vector vector)
{
    Vector result;
    result.x = matrix.m[0][0] * vector.x + matrix.m[0][1] * vector.y + matrix.m[0][2] * vector.z;
    result.y = matrix.m[1][0] * vector.x + matrix.m[1][1] * vector.y + matrix.m[1][2] * vector.z;
    result.z = matrix.m[2][0] * vector.x + matrix.m[2][1] * vector.y + matrix.m[2][2] * vector.z;
    return result;
}

void rotateCameraUp(Camera *camera)
{
    float angle = 5.0f * (SDL_PI_F / 180.0f); // Convert degrees to radians
    Matrix4x4 rotationMatrix = createRotationMatrix(camera->rightVector, angle);
    camera->direction = normalizeVector(applyMatrixToVector(rotationMatrix, camera->direction)); // Rotate direction
    camera->upVector = normalizeVector(applyMatrixToVector(rotationMatrix, camera->upVector)); // Rotate up vector
}

void rotateCameraDown(Camera *camera)
{
    float angle = -5.0f * (SDL_PI_F / 180.0f); // Convert degrees to radians
    Matrix4x4 rotationMatrix = createRotationMatrix(camera->rightVector, angle);
    camera->direction = normalizeVector(applyMatrixToVector(rotationMatrix, camera->direction)); // Rotate direction
    camera->upVector = normalizeVector(applyMatrixToVector(rotationMatrix, camera->upVector)); // Rotate up vector
}

void rotateCameraLeft(Camera *camera)
{
    float angle = -5.0f * (SDL_PI_F / 180.0f); // Convert degrees to radians
    Matrix4x4 rotationMatrix = createRotationMatrix(camera->upVector, angle);
    camera->direction = normalizeVector(applyMatrixToVector(rotationMatrix, camera->direction)); // Rotate direction
    camera->rightVector = normalizeVector(applyMatrixToVector(rotationMatrix, camera->rightVector)); // Rotate right vector
}

void rotateCameraRight(Camera *camera)
{
    float angle = 5.0f * (SDL_PI_F / 180.0f); // Convert degrees to radians
    Matrix4x4 rotationMatrix = createRotationMatrix(camera->upVector, angle);
    camera->direction = normalizeVector(applyMatrixToVector(rotationMatrix, camera->direction)); // Rotate direction
    camera->rightVector = normalizeVector(applyMatrixToVector(rotationMatrix, camera->rightVector)); // Rotate right vector
}