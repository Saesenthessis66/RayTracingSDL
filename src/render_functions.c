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
    Material objectMaterial = (Material){(SDL_Color){255,0,0,255}, 0.5f, 64.0f};
    
    // Define the ambient light material.
    LightMaterial ambientMat = {{255,255,255,255},0.5f};

    // Define the position and material of a point light.
    Vector lightPos = {0, 1, 3};
    LightMaterial lightMat = {{255,255,255,255}, 1.0f};

    // Define the position and material of a spot light.
    Vector spotlightPos = {2, 2, -3};
    Vector spotlightDir = {0, 1, -1};
    LightMaterial spotlightMat = {{255,255,255,255}, 1.0f};

    addSpotLight(scene, spotlightMat, spotlightPos, spotlightDir, 30.0f, 5.0f);
    
    // Add a point light to the scene.
    addPointLight(scene, lightMat, lightPos, 10.0f);

    // Add a plane to the scene.
    addPlane(scene, planePos, planeNormal, 10, 20, objectMaterial);
    
    // Set the ambient light in the scene.
    setAmbientLight(scene,ambientMat);
}

// Computes the color of a pixel by tracing a ray from the camera through the scene.
SDL_Color computePixelColor(int WINDOW_WIDTH, int WINDOW_HEIGHT, int x, int y, Camera* camera, Scene* scene)
{
    Ray viewRay;
    SDL_Color pixelColor;
    ObjectIntersection closestIntersection;

    // Map the pixel coordinates to a ray.
    viewRay = mapPixelToRay(camera, x, y, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Find the closest intersection of the ray with objects in the scene.
    closestIntersection = getClosestObjectIntersection(viewRay, scene);

    // Compute the color of the surface at the intersection point.
    pixelColor = computeSurfaceColor(scene, closestIntersection.point, closestIntersection.normal, viewRay.direction, closestIntersection.material);

    return pixelColor;
}

// Finds the closest intersection of a ray with objects in the scene.
ObjectIntersection getClosestObjectIntersection(Ray ray, Scene* scene)
{
    float distance;
    float minDistance = __FLT_MAX__;
    ObjectIntersection closestIntersection;
    closestIntersection.objectType = -1;
    int objectIndex;
    bool intersectionFound = false;

    // Check for intersection with all spheres in the scene.
    for (int i = 0; i < scene->objects.sphereCount; i++)
    {
        if (intersectRaySphere(ray, scene->objects.spheres[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 0;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    // Check for intersection with all planes in the scene.
    for (int i = 0; i < scene->objects.planeCount; i++)
    {
        if (intersectRayPlane(ray, scene->objects.planes[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 1;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    // Check for intersection with all triangles in the scene.
    for (int i = 0; i < scene->objects.triangleCount; i++)
    {
        if (intersectRayTriangle(ray, scene->objects.triangles[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 2;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    if (intersectionFound)
    {
        // Calculate the intersection point.
        closestIntersection.point = addVectors(ray.origin, multiplyVector(ray.direction, minDistance));
        
        // Determine the normal and material based on the object type.
        switch (closestIntersection.objectType)
        {
            case 0: // Sphere
                closestIntersection.normal = computeSphereNormal(closestIntersection.point, scene->objects.spheres[objectIndex]);
                closestIntersection.material = scene->objects.spheres[objectIndex].material;
                break;
            case 1: // Plane
                closestIntersection.normal = scene->objects.planes[objectIndex].surfaceNormal;
                closestIntersection.material = scene->objects.planes[objectIndex].material;
                break;
            case 2: // Triangle
                closestIntersection.normal = computeTriangleNormal(scene->objects.triangles[objectIndex], ray.origin);
                closestIntersection.material = scene->objects.triangles[objectIndex].material;
                break;
        }
    }
    else
    {
        // If no intersection is found, set default values.
        closestIntersection.point = (Vector){0, 0, 0};
        closestIntersection.normal = (Vector){0, 0, 0};
        closestIntersection.material = (Material){{0, 0, 0, 0}, 0, 0};
    }

    return closestIntersection;
}