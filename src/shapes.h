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
    Vector position;      // A point on the plane (origin)
    Vector surfaceNormal; // The normal vector of the plane
    Vector u;             // First direction vector (width)
    Vector v;             // Second direction vector (height)
    float width;          // Width of the  plane
    float height;         // Height of the  plane
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
    int sphereCount;
    int planeCount;
    int triangleCount;
    int maxSpheres; 
    int maxPlanes; 
    int maxTriangles; 
} Objects;

// Function to compute the normal vector at a given point on a sphere
Vector computeSphereNormal(Vector point, Sphere sphere);

// Function to compute the normal vector for a triangle
Vector computeTriangleNormal(Triangle triangle, Vector referencePoint);

// Function to check if a ray intersects a sphere
// Returns 1 if there's an intersection, 0 otherwise
// If an intersection occurs, 't' will store the distance from the ray origin
int intersectRaySphere(Ray ray, Sphere sphere, float* t);

// Function to check if a ray intersects a plane
// Returns 1 if there's an intersection, 0 otherwise
// If an intersection occurs, 't' will store the distance from the ray origin
int intersectRayPlane(Ray ray, Plane plane, float* t);

// Function to check if a ray intersects a triangle
// Returns 1 if there's an intersection, 0 otherwise
// If an intersection occurs, 't' will store the distance from the ray origin
int intersectRayTriangle(Ray ray, Triangle triangle, float* t);

#endif // SHAPES_H