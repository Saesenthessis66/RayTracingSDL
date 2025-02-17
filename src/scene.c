#include"../src/shapes.c"

typedef struct {
    Sphere* spheres;
    Plane* planes;
    Triangle* triangles;
    // lightsources arrays
    int sphereCount, planeCount, triangleCount;
} Scene;

void freeScene(Scene* scene)
{
    free(scene->spheres);
    free(scene->planes);
    free(scene->triangles);
}

void initScene(Scene* scene, int maxSpheres, int maxPlanes, int maxTriangles)
{
    scene->sphereCount = 0;
    scene->planeCount = 0;
    scene->triangleCount = 0;

    // Allocate memory dynamically
    scene->spheres = (Sphere*)malloc(maxSpheres * sizeof(Sphere));
    scene->planes = (Plane*)malloc(maxPlanes * sizeof(Plane));
    scene->triangles = (Triangle*)malloc(maxTriangles * sizeof(Triangle));

    if (!scene->spheres || !scene->planes || !scene->triangles)
    {
        printf("Error in creating scene: Memory allocation failed!\n");
        freeScene(scene);
    }
}

void addSphere(Scene* scene, Vector position, float radius, Material material, int maxSpheres)
{
    if (scene->sphereCount < maxSpheres)
    {
        scene->spheres[scene->sphereCount].position = position;
        scene->spheres[scene->sphereCount].radius = radius;
        scene->spheres[scene->sphereCount].material = material;
        scene->sphereCount++;
    }
    else
    {
        printf("Cannot add more spheres, scene limit reached!\n");
    }
}

void addPlane(Scene* scene, Vector position, Vector vector, Material material, int maxPlanes)
{
    if (scene->planeCount < maxPlanes)
    {
        scene->planes[scene->planeCount].position = position;
        scene->planes[scene->planeCount].vector = vector;
        scene->planes[scene->planeCount].material = material;
        scene->planeCount++;
    }
    else
    {
        printf("Cannot add more planes, scene limit reached!\n");
    }
}

void addTriangle(Scene* scene, Vector v1, Vector v2, Vector v3, Material material, int maxPlanes)
{
    if (scene->triangleCount < maxPlanes)
    {
        scene->triangles[scene->triangleCount].v1 = v1;
        scene->triangles[scene->triangleCount].v2 = v2;
        scene->triangles[scene->triangleCount].v3 = v3;
        scene->triangles[scene->triangleCount].material = material;
        scene->triangleCount++;
    }
    else
    {
        printf("Cannot add more triangles, scene limit reached!\n");
    }
}

// add lightsources functions