#ifndef SHAPES_H
#define SHAPES_H

#include "ray.h"  // Include the ray header to use Vector and Ray types

// Structure representing the material properties of a shape
typedef struct {
    SDL_Color color;      // RGBA color of the material
    float reflectivity;   // Reflectivity factor (0 to 1)
} Material;

// Structure representing a sphere
typedef struct {
    Vector position;      // Center of the sphere
    float radius;         // Radius of the sphere
    Material material;    // Material properties of the sphere
} Sphere;

// Structure representing a plane
typedef struct {
    Vector position;      // A point on the plane
    Vector vector;        // A vector defining the plane's orientation
    Vector surfaceNormal; // The normal vector of the plane (should be a unit vector)
    Material material;    // Material properties of the plane
} Plane;

// Structure representing a triangle
typedef struct {
    Vector v1, v2, v3;    // The three vertices of the triangle
    Material material;    // Material properties of the triangle
} Triangle;

// Structure that holds every object type
typedef struct {
    Sphere* spheres;
    Plane* planes;
    Triangle* triangles;
    int sphereCount, planeCount, triangleCount;
} Objects;

// Function to compute the normal vector at a given point on a sphere
Vector computeSphereNormal(Vector point, Sphere sphere);

// Function to compute the normal vector for a triangle
Vector computeTriangleNormal(Triangle triangle, Vector referencePoint);

#endif // SHAPES_H