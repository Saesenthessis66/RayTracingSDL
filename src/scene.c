#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

void initScene(Scene* scene, int maxSpheres, int maxPlanes, int maxTriangles)
{
    scene->objects.sphereCount = 0;
    scene->objects.planeCount = 0;
    scene->objects.triangleCount = 0;

    // Allocate memory dynamically
    scene->objects.spheres = malloc(maxSpheres * sizeof(Sphere));
    scene->objects.planes = malloc(maxPlanes * sizeof(Plane));
    scene->objects.triangles = malloc(maxTriangles * sizeof(Triangle));

    if (!scene->objects.spheres || !scene->objects.planes || !scene->objects.triangles)
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

void addPlane(Scene* scene, Vector position, Vector vector, Material material, int maxPlanes)
{
    if (scene->objects.planeCount < maxPlanes)
    {
        scene->objects.planes[scene->objects.planeCount].position = position;
        scene->objects.planes[scene->objects.planeCount].vector = vector;
        scene->objects.planes[scene->objects.planeCount].material = material;
        scene->objects.planeCount++;
    }
    else
    {
        printf("Cannot add more planes, scene limit reached!\n");
    }
}

void addTriangle(Scene* scene, Vector v1, Vector v2, Vector v3, Material material, int maxPlanes)
{
    if (scene->objects.triangleCount < maxPlanes)
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