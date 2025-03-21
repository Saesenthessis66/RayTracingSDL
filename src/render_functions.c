#include "render_functions.h"

// Initializes the scene with default objects and lighting.
void initialize_scene(int WINDOW_WIDTH, int WINDOW_HEIGHT, Camera* camera, Scene* scene)
{
    // Initialize the camera with a specific position, direction, and field of view.
    initCamera(camera, (Vector){0, 5, 5}, (Vector){0, -0.2f, -1}, (Vector){0, 1, 0}, 90.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Initialize the scene with default parameters.
    initScene(scene, 5, 5, 5, 5, 5, 5);

    // Define the position and normal of a plane.
    Vector planePos = {0, 0, 3};
    Vector planeNormal = {0, 1, 0};
    
    // Define the material properties of the plane.
    Material objectMaterial = (Material){(SDL_Color){255,0,0,255}, 1.0f, 128.0f};
    
    // Define the ambient light material.
    LightMaterial ambientMat = {{255,255,255,255},0.5f};

    // Define the position and material of a point light.
    Vector lightPos = {0, 1, 3};
    LightMaterial lightMat = {{255,255,255,255}, 1.0f};

    // Define the position and material of a spot light.
    Vector spotlightPos = {3, 2, -5};
    Vector spotlightDir = {0, 1, -1};
    LightMaterial spotlightMat = {{255,255,255,255}, 1.0f};

    addSpotLight(scene, spotlightMat, spotlightPos, spotlightDir, 30.0f, 2.0f);
    
    // Add a point light to the scene.
    addPointLight(scene, lightMat, lightPos, 10.0f);

    // Add a plane to the scene.
    addPlane(scene, planePos, planeNormal, 10, 20, objectMaterial);
    
    // Set the ambient light in the scene.
    setAmbientLight(scene,ambientMat);

    Vector p1 = {-2, 0, -2};
    Vector p2 = {2, 0, -2};
    Vector p3 = {0, 4, -4};
    Material triangleMaterial = (Material){(SDL_Color){0,255,0,255}, 1.0f, 128.0f};
    addTriangle(scene, p1, p2, p3, triangleMaterial);

    scene->bvhRoot = buildBVH(&scene->objects);
}

// Computes the color of a pixel by tracing a ray from the camera through the scene.
SDL_Color computePixelColor(int WINDOW_WIDTH, int WINDOW_HEIGHT, int x, int y, Camera* camera, Scene* scene)
{
    Ray viewRay;
    SDL_Color pixelColor;
    ObjectIntersection closestIntersection;
    closestIntersection.material = (Material){{0, 0, 0, 0}, 0, 0};

    // Map the pixel coordinates to a ray.
    viewRay = mapPixelToRay(camera, x, y, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Find the closest intersection of the ray with objects in the scene.
    intersectBVH(viewRay, scene->bvhRoot, &closestIntersection);

    // Compute the color of the surface at the intersection point.
    pixelColor = computeSurfaceColor(scene, closestIntersection.point, closestIntersection.normal, viewRay.direction, closestIntersection.material);

    return pixelColor;
}