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
    // Compute the triangle's normal
    Vector triangleNormal = computeTriangleNormal(triangle, ray.origin);

    // Compute the dot product between the normal and the ray direction
    float normalDotRayDir = dotProduct(triangleNormal, ray.direction);

    // If the dot product is 0, the ray is parallel to the triangle (no intersection)
    if (normalDotRayDir == 0) return 0;

    // Compute the intersection distance along the ray
    float intersectionDistance = dotProduct(subtractVectors(triangle.v1, ray.origin), triangleNormal) / normalDotRayDir;

    // If the intersection distance is negative, the triangle is behind the ray origin (no valid intersection)
    if (intersectionDistance < 0) return 0;

    // Compute the intersection point
    Vector intersectionPoint = addVectors(ray.origin, multiplyVector(ray.direction, intersectionDistance));

    // Compute edges of the triangle
    Vector edge1 = subtractVectors(triangle.v2, triangle.v1);
    Vector edge2 = subtractVectors(triangle.v3, triangle.v1);
    Vector vector = subtractVectors(intersectionPoint, triangle.v1);

    // Compute dot products for barycentric coordinates
    float dot00 = dotProduct(edge1, edge1);
    float dot01 = dotProduct(edge1, edge2);
    float dot02 = dotProduct(edge1, vector);
    float dot11 = dotProduct(edge2, edge2);
    float dot12 = dotProduct(edge2, vector);

    // Compute determinant
    float determinant = (dot00 * dot11) - (dot01 * dot01);

    // If determinant is zero, the triangle is degenerate (no valid intersection)
    if (determinant == 0) return 0;

    // Compute barycentric coordinates
    float invDeterminant = 1 / determinant;
    float beta = ((dot11 * dot02) - (dot01 * dot12)) * invDeterminant;
    float gamma = ((dot00 * dot12) - (dot01 * dot02)) * invDeterminant;
    float alpha = 1.0f - beta - gamma;

    // Check if the intersection point is inside the triangle
    if (alpha >= 0 && beta >= 0 && gamma >= 0)
    {
        *t = intersectionDistance;
        return 1; // Intersection occurred
    }

    return 0; // No intersection
}
