#ifndef BVH_H
#define BVH_H

#include "ray.h"    // For Vector
#include "shapes.h" // For Object types

// Axis-Aligned Bounding Box (AABB)
typedef struct {
    Vector min;  // Minimum corner
    Vector max;  // Maximum corner
} AABB;

// Forward declaration of Scene
struct Scene;

// BVH Node Structure
typedef struct BVHNode{
    AABB bounds;           // Bounding box
    struct BVHNode* left;  // Left child
    struct BVHNode* right; // Right child
    Objects objects;       // Objects in the leaf node
} BVHNode;

// Structure to store an object along with its center and type
struct ObjectArray
{
    Vector center; // Center of the object (used for sorting)
    void* object; // Pointer to the actual object (Plane, Sphere, or Triangle)
    int type; // 0 = Plane, 1 = Sphere, 2 = Triangle
};

typedef struct {
    Vector point; // The intersection point
    Vector normal; // The normal at the intersection point
    Material material; // The material of the intersected object
    int objectType; // The type of the intersected object
} ObjectIntersection;

// Compute AABB for a single sphere
AABB computeSphereAABB(Sphere* sphere);

// Compute AABB for a single plane
AABB computePlaneAABB(Plane* plane);

// Compute AABB for a single triangle
AABB computeTriangleAABB(Triangle* triangle);

// Build the BVH Tree
BVHNode* buildBVH(Objects* objects);

// Check for intersection between Ray and AABB
float intersectAABB(Ray ray, AABB box);

// Recursively checks for ray intersections with a BVH (Bounding Volume Hierarchy) tree.
int intersectBVH(Ray ray, BVHNode* node, ObjectIntersection* hit);

#endif // BVH_H