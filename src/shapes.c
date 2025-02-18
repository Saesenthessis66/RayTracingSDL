#include "shapes.h"

// Computes the normal vector at a given point on a sphere
// Formula: N = (P - C) / |P - C|
Vector computeSphereNormal(Vector point, Sphere sphere)
{
    return normalizeVector(subtractVectors(point, sphere.position));
}

// Computes the normal vector for a triangle
// 1. Compute two edges: edge1 = v2 - v1, edge2 = v3 - v1
// 2. Compute the normal using the cross product of edge1 and edge2
// 3. Check if the normal faces the reference point
// 4. If the normal is flipped, reverse it
Vector computeTriangleNormal(Triangle triangle, Vector referencePoint)
{
    Vector edge1 = subtractVectors(triangle.v2, triangle.v1);
    Vector edge2 = subtractVectors(triangle.v3, triangle.v1);

    Vector normal = normalizeVector(vectorCrossProduct(edge1, edge2));

    // Check if the normal is facing the reference point
    Vector toReference = subtractVectors(referencePoint, triangle.v1);
    
    // Flip the normal if it's pointing in the wrong direction
    if (dotProduct(normal, toReference) < 0)
    {
        normal = multiplyVector(normal, -1);
    }

    return normal;
}