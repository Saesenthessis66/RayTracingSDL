#ifndef BVH_H
#define BVH_H

#include "render_functions.h"

/// Axis-Aligned Bounding Box (AABB)
typedef struct {
    Vector min;  // Minimum corner
    Vector max;  // Maximum corner
} AABB;

/// BVH Node Structure
typedef struct BVHNode {
    AABB bounds;           // Bounding box
    struct BVHNode* left;  // Left child
    struct BVHNode* right; // Right child
    int objectIndex;       // Index of the object (only in leaf nodes)
} BVHNode;

// Compute AABB for a single sphere
AABB computeSphereAABB(Sphere* sphere);

// Compute AABB for a single plane
AABB computePlaneAABB(Plane* plane);

// Compute AABB for a single triangle
AABB computeTriangleAABB(Triangle* triangle);

// Build the BVH Tree
BVHNode* buildBVH(Objects* objects, int start, int end);

// Check for intersection between Ray and AABB
int intersectAABB(Ray ray, AABB box);

// Check for intersection between Ray and BVH Tree
int intersectBVH(Ray ray, BVHNode* node, ObjectIntersection* hit, Objects* objects);

#endif // BVH_H