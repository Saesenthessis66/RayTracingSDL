#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.001 // small offset

void initScene(Scene* scene, int maxSpheres, int maxPlanes, int maxTriangles, int maxPointLights, int maxDirectionalLights, int maxSpotLights)
{
    scene->objects.sphereCount = 0;
    scene->objects.planeCount = 0;
    scene->objects.triangleCount = 0;

    scene->lights.directionalLightCount = 0;
    scene->lights.pointLightCount = 0;
    scene->lights.spotLightCount = 0;

    // Allocate memory dynamically
    scene->objects.spheres = malloc(maxSpheres * sizeof(Sphere));
    scene->objects.planes = malloc(maxPlanes * sizeof(Plane));
    scene->objects.triangles = malloc(maxTriangles * sizeof(Triangle));

    scene->lights.directionalLights = malloc(maxDirectionalLights * sizeof(DirectionalLight));
    scene->lights.pointLights = malloc(maxPointLights * sizeof(PointLight));
    scene->lights.spotLights = malloc(maxSpotLights * sizeof(SpotLight));

    scene->objects.maxSpheres = maxSpheres; 
    scene->objects.maxPlanes = maxPlanes; 
    scene->objects.maxTriangles = maxTriangles; 
    
    scene->lights.maxPointLights = maxPointLights; 
    scene->lights.maxDirectionalLights = maxDirectionalLights; 
    scene->lights.maxSpotLights = maxSpotLights;


    if (!scene->objects.spheres || !scene->objects.planes || !scene->objects.triangles
         || !scene->lights.directionalLights || !scene->lights.pointLights || !scene->lights.spotLights)
    {
        printf("Error in creating scene: Memory allocation failed!\n");
        freeScene(scene);
    }
}

void freeScene(Scene* scene)
{
    free(scene->objects.spheres);
    free(scene->objects.planes);
    free(scene->objects.triangles);

    free(scene->lights.directionalLights);
    free(scene->lights.pointLights);
    free(scene->lights.spotLights);
}

void addSphere(Scene* scene, Vector position, float radius, Material material)
{
    if (scene->objects.sphereCount < scene->objects.maxSpheres)
    {
        scene->objects.spheres[scene->objects.sphereCount].position = position;
        scene->objects.spheres[scene->objects.sphereCount].radius = radius;
        scene->objects.spheres[scene->objects.sphereCount].material = material;
        scene->objects.sphereCount++;
    }
    else
    {
        printf("Cannot add more spheres, scene limit reached!\n");
    }
}

void addPlane(Scene* scene, Vector position, Vector normal, float width, float height, Material material) 
{
    if (scene->objects.planeCount < scene->objects.maxPlanes) 
    {
        Plane* plane = &scene->objects.planes[scene->objects.planeCount];

        // Set plane properties
        plane->position = position;
        plane->surfaceNormal = normalizeVector(normal);
        plane->width = width;
        plane->height = height;
        plane->material = material;

        // Compute basis vectors (u, v) perpendicular to the normal
        Vector arbitrary = {1, 0, 0}; // Default choice
        if (SDL_abs(plane->surfaceNormal.x) > 0.9f)  // If normal is too close to (1,0,0), pick another vector
            arbitrary = (Vector){0, 1, 0};

        plane->u = normalizeVector(vectorCrossProduct(arbitrary, plane->surfaceNormal));  // First tangent vector
        plane->v = vectorCrossProduct(plane->surfaceNormal, plane->u);  // Second tangent vector (already normalized)

        scene->objects.planeCount++;
    } 
    else 
    {
        printf("Cannot add more planes, scene limit reached!\n");
    }
}


void addTriangle(Scene* scene, Vector v1, Vector v2, Vector v3, Material material)
{
    if (scene->objects.triangleCount < scene->objects.maxTriangles)
    {
        scene->objects.triangles[scene->objects.triangleCount].v1 = v1;
        scene->objects.triangles[scene->objects.triangleCount].v2 = v2;
        scene->objects.triangles[scene->objects.triangleCount].v3 = v3;
        scene->objects.triangles[scene->objects.triangleCount].material = material;
        scene->objects.triangleCount++;
    }
    else
    {
        printf("Cannot add more triangles, scene limit reached!\n");
    }
}

void addPointLight(Scene *scene, LightMaterial material, Vector position, float range)
{
    if(scene->lights.pointLightCount < scene->lights.maxPointLights)
    {
        scene->lights.pointLights[scene->lights.pointLightCount].position = position;
        scene->lights.pointLights[scene->lights.pointLightCount].material = material;
        scene->lights.pointLights[scene->lights.pointLightCount].range = range;
        scene->lights.pointLightCount++;
    }
    else
    {
        printf("Cannot add more point lights, scene limit reached!\n");
    }
}

void addDirectionalLight(Scene *scene, LightMaterial material, Vector position, Vector direction)
{
    if(scene->lights.directionalLightCount < scene->lights.maxDirectionalLights)
    {
        scene->lights.directionalLights[scene->lights.directionalLightCount].position = position;
        scene->lights.directionalLights[scene->lights.directionalLightCount].material = material;
        scene->lights.directionalLights[scene->lights.directionalLightCount].direction = direction;
        scene->lights.directionalLightCount++;
    }
    else
    {
        printf("Cannot add more directional lights, scene limit reached!\n");
    }
}

void addSpotLight(Scene *scene, LightMaterial material, Vector position, Vector direction, float cutOffAngle, float innerCutoffAngle)
{
    if(scene->lights.spotLightCount < scene->lights.maxSpotLights)
    {
        scene->lights.spotLights[scene->lights.spotLightCount].position = position;
        scene->lights.spotLights[scene->lights.spotLightCount].material = material; 
        scene->lights.spotLights[scene->lights.spotLightCount].direction = direction;
        scene->lights.spotLights[scene->lights.spotLightCount].cutoffAngle = cutOffAngle;
        scene->lights.spotLights[scene->lights.spotLightCount].innerCutoffAngle = innerCutoffAngle;
        scene->lights.spotLightCount++;
    }
    else
    {
        printf("Cannot add more spot lights, scene limit reached!\n");
    }
}

void setAmbientLight(Scene *scene, LightMaterial material)
{
    scene->lights.ambientLight.material = material;
}

int isPointInShadow(Vector point, PointLight *light, Scene *scene)
{
    // Compute the direction from the point to the light source
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));

    // Apply a small offset to prevent self-shadowing artifacts (avoid floating-point errors)
    Vector offset = multiplyVector(lightDirection, EPSILON);
    
    // Create a shadow ray that starts just above the surface and points toward the light
    Ray shadowRay = {addVectors(point, offset), lightDirection};

    // Compute the maximum possible distance the shadow ray can travel before reaching the light
    float maxDistance = vectorLength(subtractVectors(light->position, point));

    float distance; // Variable to store the intersection distance

    // Check for intersection with all spheres in the scene
    for (int i = 0; i < scene->objects.sphereCount; i++)
    {
        // If the shadow ray hits a sphere and the hit is before the light, the point is in shadow
        if (intersectRaySphere(shadowRay, scene->objects.spheres[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all planes in the scene
    for (int i = 0; i < scene->objects.planeCount; i++)
    {
        if (intersectRayPlane(shadowRay, scene->objects.planes[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all triangles in the scene
    for (int i = 0; i < scene->objects.triangleCount; i++)
    {
        if (intersectRayTriangle(shadowRay, scene->objects.triangles[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // If no object is blocking the light, the point is fully illuminated
    return 0;
}

int isPointInShadowDir(Vector point, DirectionalLight *light, Scene *scene)
{        
    // Create a small offset in the direction of the light to prevent self-shadowing
    Ray shadowRay = {addVectors(point, multiplyVector(light->direction, EPSILON)),light->direction};

    float distance; // Variable to store the intersection distance

    // Check for intersection with all spheres in the scene
    for (int i = 0; i < scene->objects.sphereCount; i++)
    {
        // If the shadow ray intersects a sphere, the point is in shadow
        if (intersectRaySphere(shadowRay, scene->objects.spheres[i], &distance))
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all planes in the scene
    for (int i = 0; i < scene->objects.planeCount; i++)
    {
        // If the shadow ray intersects a plane, the point is in shadow
        if (intersectRayPlane(shadowRay, scene->objects.planes[i], &distance))
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all triangles in the scene
    for (int i = 0; i < scene->objects.triangleCount; i++)
    {
        // If the shadow ray intersects a triangle, the point is in shadow
        if (intersectRayTriangle(shadowRay, scene->objects.triangles[i], &distance))
        {
            return 1; // The point is in shadow
        }
    }
    
    // If no object is blocking the light, the point is fully illuminated
    return 0;
}


int isPointInShadowSpot(Vector point, SpotLight *light, Scene *scene)
{
    // Compute the direction from the point to the light source
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));

    // Compute angle between spotlight direction and vector the point to the light source
    float cosAngle = dotProduct(light->direction, normalizeVector(lightDirection));

    // Check if point is in spotlight range
    if(cosAngle > SDL_cosf(light->cutoffAngle * SDL_PI_F / 180)) return 0;

    // Apply a small offset to prevent self-shadowing artifacts (avoid floating-point errors)
    Vector offset = multiplyVector(lightDirection, EPSILON);
    
    // Create a shadow ray that starts just above the surface and points toward the light
    Ray shadowRay = {addVectors(point, offset), lightDirection};

    // Compute the maximum possible distance the shadow ray can travel before reaching the light
    float maxDistance = vectorLength(subtractVectors(light->position, point));

    float distance; // Variable to store the intersection distance

    // Check for intersection with all spheres in the scene
    for (int i = 0; i < scene->objects.sphereCount; i++)
    {
        // If the shadow ray hits a sphere and the hit is before the light, the point is in shadow
        if (intersectRaySphere(shadowRay, scene->objects.spheres[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all planes in the scene
    for (int i = 0; i < scene->objects.planeCount; i++)
    {
        if (intersectRayPlane(shadowRay, scene->objects.planes[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // Check for intersection with all triangles in the scene
    for (int i = 0; i < scene->objects.triangleCount; i++)
    {
        if (intersectRayTriangle(shadowRay, scene->objects.triangles[i], &distance) && 
            distance > 0 && distance < maxDistance) 
        {
            return 1; // The point is in shadow
        }
    }

    // If no object is blocking the light, the point is fully illuminated
    return 0;
}
