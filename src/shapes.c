#include "shapes.h"

#define EPSILON 0.0001

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

int intersectRaySphere(Ray ray, Sphere sphere, float *t)
{
    Vector sphereToRay = subtractVectors(sphere.position, ray.origin); // Vector from ray origin to sphere center
    float projectionLength = dotProduct(sphereToRay, ray.direction);   // Project onto ray direction

    if (projectionLength < 0) return 0; // Sphere is behind the ray

    float closestApproachSquared = dotProduct(sphereToRay, sphereToRay) - projectionLength * projectionLength;

    if (closestApproachSquared > sphere.radius * sphere.radius) return 0; // Ray misses the sphere

    float intersectionOffset = SDL_sqrtf(sphere.radius * sphere.radius - closestApproachSquared);

    float t0 = projectionLength - intersectionOffset; // First intersection point
    float t1 = projectionLength + intersectionOffset; // Second intersection point

    if (t0 >= 0)
    {
        *t = t0; // Closest valid intersection
        return 1;
    }
    else if (t1 >= 0)
    {
        *t = t1; // Ray starts inside the sphere, return the exit point
        return 1;
    }

    return 0;
}

int intersectRayPlane(Ray ray, Plane plane, float *t)
{
    // Compute the dot product of the ray direction and the plane normal
    float normalDotRayDir = dotProduct(ray.direction, plane.surfaceNormal);

    // If the denominator is zero, the ray is parallel to the plane (no intersection)
    if (normalDotRayDir == 0) return 0;

    // Compute the distance along the ray to the intersection point
    float intersectionDistance = dotProduct(subtractVectors(plane.position, ray.origin), plane.surfaceNormal) / normalDotRayDir;

    // If the intersection distance is negative, the intersection is behind the ray origin (no valid intersection)
    if (intersectionDistance < 0) return 0;

    // Compute the actual intersection point on the infinite plane
    Vector intersectionPoint = addVectors(ray.origin, multiplyVector(ray.direction, intersectionDistance));

    // Convert the intersection point into the local plane coordinate system
    Vector relativePoint = subtractVectors(intersectionPoint, plane.position);

    // Compute the u and v coordinates (local 2D plane coordinates)
    float uCoord = dotProduct(relativePoint, plane.u);
    float vCoord = dotProduct(relativePoint, plane.v);

    // Check if the intersection point is within the finite plane bounds
    if (SDL_abs(uCoord) <= plane.width / 2 && SDL_abs(vCoord) <= plane.height / 2)
    {
        *t = intersectionDistance; // Store the intersection distance
        return 1; // Intersection occurred
    }

    return 0; // Intersection is outside the plane bounds
}

int intersectRayTriangle(Ray ray, Triangle triangle, float *t)
{
    // Compute the two edges of the triangle
    Vector edge1 = subtractVectors(triangle.v2, triangle.v1);
    Vector edge2 = subtractVectors(triangle.v3, triangle.v1);

    // Compute the determinant using the cross product of ray direction and edge2
    Vector pVector = vectorCrossProduct(ray.direction, edge2);
    float determinant = dotProduct(edge1, pVector);

    // Check if the determinant is close to zero (ray is parallel to the triangle)
    if (SDL_fabs(determinant) < EPSILON) return 0;

    // Compute the inverse determinant for efficiency
    float invDeterminant = 1.0f / determinant;

    // Compute the vector from the triangle's first vertex to the ray origin
    Vector tVector = subtractVectors(ray.origin, triangle.v1);

    // Compute barycentric coordinate u
    float u = dotProduct(tVector, pVector) * invDeterminant;
    if (u < 0 || u > 1) return 0; // Outside the triangle

    // Compute qVector (cross product of tVector and edge1)
    Vector qVector = vectorCrossProduct(tVector, edge1);

    // Compute barycentric coordinate v
    float v = dotProduct(ray.direction, qVector) * invDeterminant;
    if (v < 0 || (u + v) > 1) return 0; // Outside the triangle

    // Compute the intersection distance t along the ray
    *t = dotProduct(edge2, qVector) * invDeterminant;

    // Ensure the intersection is in front of the ray origin (not behind)
    if (*t < 0) return 0;

    return 1; // Intersection detected
}
