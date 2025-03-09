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
    Objects* objects;      // Pointer to objects (only in leaf nodes)
    int objectCount;       // Number of objects in this node
} BVHNode;

// Compute AABB for a single sphere
AABB computeSphereAABB(Sphere* sphere);

// Compute AABB for a single plane (optional, if needed)
AABB computePlaneAABB(Plane* plane);

// Compute AABB for a single triangle
AABB computeTriangleAABB(Triangle* triangle);

// Build the BVH Tree
BVHNode* buildBVH(Objects* objects, int start, int end);

// Intersect Ray with BVH
int intersectBVH(Ray ray, BVHNode* node, ObjectIntersection* hit);

#endif // BVH_H
