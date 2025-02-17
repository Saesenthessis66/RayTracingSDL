#include "../src/ray.c"

typedef struct
{
    Vector position;
    float radius;
    SDL_Color color; // r g b a
    float reflectivity;
    Vector surfaceNormal;
} Sphere;

typedef struct
{
    Vector position;
    Vector vector;
    SDL_Color color;
    float reflectivity;
    Vector surfaceNormal;
} Plane;

typedef struct
{
    Vector v1, v2, v3;
    SDL_Color color;
    float reflectivity;
    Vector surfaceNormal;
} Triangle;

