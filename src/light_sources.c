#include "../src/ray.c"

typedef struct{
    SDL_Color color;
    float intensity;
}LightMaterial;

typedef struct{
    Vector position;
    float range; // Based on range calculate intensity of point light for example if object is 3 units away and range is 6 intensity eqals 0.5
    LightMaterial material;
} PointLight;

typedef struct{

} DirectionalLight;
    Vector position;
    Vector direction;
    LightMaterial material;
typedef struct{
    Vector position;
    LightMaterial material;

} SpotLight;

typedef struct{
    LightMaterial material;
} AmbientLight;
