#include <SDL3/SDL.h>

typedef struct{
    float x, y, z;
} Vector;

typedef struct {
    Vector origin; 
    Vector direction;
} Ray;

typedef struct {
    Vector position;
    Vector direction; 
    Vector upVector;  
    Vector rightVector;
    float fieldOfView; 
    float aspectRatio; 
} Camera;

float vectorLength(Vector v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector multiplyVector(Vector v, float multiplier)
{
    v.x *= multiplier;
    v.y *= multiplier;
    v.z *= multiplier;

    return v;
}

Vector addVectors(Vector u, Vector v)
{
    u.x += v.x;
    u.y += v.y;
    u.z += v.z;

    return u;
}

Vector normalizeVector(Vector v) {
    float l = vectorLength(v);
    if (l == 0) { 
        printf("Cannot normalize a zero vector!\n");
        return v;
    }
    Vector result = {v.x / l, v.y / l, v.z / l};
    return result;
}

Vector vectorCrossProduct(Vector u, Vector v)
{
    Vector cross;

    cross.x = u.y * v.z - u.z * v.y;
    cross.y = u.z * v.x - u.x * v.z;
    cross.z = u.x * v.y - u.y * v.x;

    return cross;
}

void initCamera(Camera* camera, Vector pos, Vector dir, Vector up, float fov, int screenWidth, int screenHeight)
{
    camera->position = pos;
    camera->direction = dir;
    camera->upVector = up;
    camera ->fieldOfView = fov;
    camera->aspectRatio = screenWidth/screenHeight;

    camera->rightVector = vectorCrossProduct(up, dir);
}

Ray* mapPixelToRay(Camera* camera, int pixelX, int pixelY, int screenWidth, int screenHeight)
{
    float computedX, computedY, scale;
    
    computedX = 2 * (pixelX + 0.5) / screenWidth - 1;
    computedY = 1 - 2 * (pixelY + 0.5) / screenHeight;
    scale = SDL_tan(camera->fieldOfView / 2);
    computedX = computedX * camera->aspectRatio * scale;
    computedY = computedY * scale;

    Ray* ray = malloc(sizeof(Ray));

    ray->origin = camera->position;

    Vector dirX = multiplyVector(camera->rightVector, computedX);
    Vector dirY = multiplyVector(camera->upVector, computedY);
    Vector dirSum = addVectors(dirX, dirY);

    ray->direction = addVectors(dirSum, camera->direction);

    ray->direction = normalizeVector(ray->direction);
    
    return ray;
}