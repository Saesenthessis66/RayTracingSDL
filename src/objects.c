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
    float fieldOfView; 
    float aspectRatio; 
} Camera;

void initCamera(Camera* camera, Vector position, Vector direction, Vector upVector)
{
    
}