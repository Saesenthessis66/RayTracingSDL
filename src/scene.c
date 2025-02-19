#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

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

void addSphere(Scene* scene, Vector position, float radius, Material material, int maxSpheres)
{
    if (scene->objects.sphereCount < maxSpheres)
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

void addPlane(Scene* scene, Vector position, Vector normal, float width, float height, Material material, int maxPlanes) 
{
    if (scene->objects.planeCount < maxPlanes) 
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
        if (fabs(plane->surfaceNormal.x) > 0.9f)  // If normal is too close to (1,0,0), pick another vector
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


void addTriangle(Scene* scene, Vector v1, Vector v2, Vector v3, Material material, int maxTriangles)
{
    if (scene->objects.triangleCount < maxTriangles)
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

void addPointLight(Scene *scene, LightMaterial material, Vector position, float range, int maxPointLights)
{
    if(scene->lights.pointLightCount < maxPointLights)
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

void addDirectionalLight(Scene *scene, LightMaterial material, Vector position, Vector direction, int maxDirectionalLights)
{
    if(scene->lights.directionalLightCount < maxDirectionalLights)
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

void addSpotLight(Scene *scene, LightMaterial material, Vector position, Vector direction, float cutOffAngle, int maxSpotLights)
{
    if(scene->lights.spotLightCount < maxSpotLights)
    {
        scene->lights.spotLights[scene->lights.spotLightCount].position = position;
        scene->lights.spotLights[scene->lights.spotLightCount].material = material; 
        scene->lights.spotLights[scene->lights.spotLightCount].direction = direction;
        scene->lights.spotLights[scene->lights.spotLightCount].cutoffAngle = cutOffAngle;
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
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));
    

    return 0;
}

int isPointInShadowDir(Vector point, DirectionalLight *light, Scene *scene)
{
    return 0;
}

int isPointInShadowSpot(Vector point, SpotLight *light, Scene *scene)
{
    return 0;
}
