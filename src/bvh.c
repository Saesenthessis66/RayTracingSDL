#include "bvh.h"

AABB computeSphereAABB(Sphere* sphere) {
    AABB box;
    box.min.x = sphere->position.x - sphere->radius;
    box.min.y = sphere->position.y - sphere->radius;
    box.min.z = sphere->position.z - sphere->radius;

    box.max.x = sphere->position.x + sphere->radius;
    box.max.y = sphere->position.y + sphere->radius;
    box.max.z = sphere->position.z + sphere->radius;

    return box;
}

AABB computePlaneAABB(Plane* plane) {
    AABB box;
    box.min.x = plane->position.x - plane->width / 2;
    box.min.y = plane->position.y;
    box.min.z = plane->position.z - plane->height / 2;

    box.max.x = plane->position.x + plane->width / 2;
    box.max.y = plane->position.y;
    box.max.z = plane->position.z + plane->height / 2;

    return box;
}

AABB computeTriangleAABB(Triangle* triangle) {
    AABB box;
    box.min.x = SDL_min(SDL_min(triangle->v1.x, triangle->v2.x), triangle->v3.x);
    box.min.y = SDL_min(SDL_min(triangle->v1.y, triangle->v2.y), triangle->v3.y);
    box.min.z = SDL_min(SDL_min(triangle->v1.z, triangle->v2.z), triangle->v3.z);

    box.max.x = SDL_min(SDL_min(triangle->v1.x, triangle->v2.x), triangle->v3.x);
    box.max.y = SDL_min(SDL_min(triangle->v1.y, triangle->v2.y), triangle->v3.y);
    box.max.z = SDL_min(SDL_min(triangle->v1.z, triangle->v2.z), triangle->v3.z);

    return box;
}

// Helper function to compute the AABB for a set of objects
AABB computeAABB(Objects *objects, int start, int end) {
    AABB box;
    box.min.x = box.min.y = box.min.z = __FLT_MAX__;
    box.max.x = box.max.y = box.max.z = -__FLT_MAX__;

    for (int i = start; i < end; i++) {
        AABB objectBox;
        if (i < objects->sphereCount) {
            objectBox = computeSphereAABB(&objects->spheres[i]);
        } else if (i < objects->sphereCount + objects->planeCount) {
            objectBox = computePlaneAABB(&objects->planes[i - objects->sphereCount]);
        } else {
            objectBox = computeTriangleAABB(&objects->triangles[i - objects->sphereCount - objects->planeCount]);
        }
        box.min.x = SDL_min(box.min.x, objectBox.min.x);
        box.min.y = SDL_min(box.min.y, objectBox.min.y);
        box.min.z = SDL_min(box.min.z, objectBox.min.z);
        box.max.x = SDL_max(box.max.x, objectBox.max.x);
        box.max.y = SDL_max(box.max.y, objectBox.max.y);
        box.max.z = SDL_max(box.max.z, objectBox.max.z);
    }
    return box;
}

BVHNode *buildBVH(Objects *objects, int start, int end) {
    BVHNode *node = (BVHNode *)malloc(sizeof(BVHNode));
    node->bounds = computeAABB(objects, start, end);

    if (end - start == 1) {
        // Leaf node
        node->left = node->right = NULL;
        node->objectIndex = start;
    } else {
        // Internal node
        int mid = (start + end) / 2;
        node->left = buildBVH(objects, start, mid);
        node->right = buildBVH(objects, mid, end);
        node->objectIndex = -1;
    }

    return node;
}

int intersectAABB(Ray ray, AABB box)
{
    return 0;
}

int intersectBVH(Ray ray, BVHNode *node, ObjectIntersection *hit, Objects *objects)
{
    return 0;
}
