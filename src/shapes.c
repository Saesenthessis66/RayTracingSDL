#include "../src/ray.c"

typedef struct {
    SDL_Color color;
    float reflectivity;
} Material;

typedef struct {
    Vector position;
    float radius;
    Material material;
} Sphere;

typedef struct {
    Vector position;
    Vector vector;
    Vector surfaceNormal;
    Material material;
} Plane;

typedef struct {
    Vector v1, v2, v3;
    Material material;
} Triangle;

Vector computeSphereNormal(Vector point, Sphere sphere)
{
    return normalizeVector(subtractVectors(point, sphere.position));
}

Vector computeTriangleNormal(Triangle triangle, Vector referencePoint)
{
    Vector edge1 = subtractVectors(triangle.v2, triangle.v1);
    Vector edge2 = subtractVectors(triangle.v3, triangle.v1);

    Vector normal = normalizeVector(vectorCrossProduct(edge1, edge2));

    // Check if the normal is facing the reference point
    Vector toReference = subtractVectors(referencePoint, triangle.v1);
    
    // If the normal and reference direction are in opposite directions, flip the normal
    if (dotProduct(normal, toReference) < 0)
    {
        normal = multiplyVector(normal, -1);
    }

    return normal;
}

// intersect function object and ray