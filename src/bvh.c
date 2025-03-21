#include "bvh.h"
#include <stdlib.h>

AABB computeSphereAABB(Sphere* sphere) 
{
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

    // Compute half-extents
    float halfWidth = plane->width / 2.0f;
    float halfHeight = plane->height / 2.0f;

    // Compute the four corners of the plane
    Vector corners[4];
    corners[0] = addVectors(plane->position, addVectors(multiplyVector(plane->u, halfWidth), multiplyVector(plane->v, halfHeight)));
    corners[1] = addVectors(plane->position, addVectors(multiplyVector(plane->u, halfWidth), multiplyVector(plane->v, -halfHeight)));
    corners[2] = addVectors(plane->position, addVectors(multiplyVector(plane->u, -halfWidth), multiplyVector(plane->v, halfHeight)));
    corners[3] = addVectors(plane->position, addVectors(multiplyVector(plane->u, -halfWidth), multiplyVector(plane->v, -halfHeight)));

    // Compute AABB min/max
    box.min = corners[0];
    box.max = corners[0]; 
    for (int i = 1; i < 4; i++) {
        box.min.x = SDL_min(box.min.x, corners[i].x);
        box.min.y = SDL_min(box.min.y, corners[i].y);
        box.min.z = SDL_min(box.min.z, corners[i].z);

        box.max.x = SDL_max(box.max.x, corners[i].x);
        box.max.y = SDL_max(box.max.y, corners[i].y);
        box.max.z = SDL_max(box.max.z, corners[i].z);
    }

    return box;
}

AABB computeTriangleAABB(Triangle* triangle)
{
    AABB box;
    box.min.x = SDL_min(SDL_min(triangle->v1.x, triangle->v2.x), triangle->v3.x);
    box.min.y = SDL_min(SDL_min(triangle->v1.y, triangle->v2.y), triangle->v3.y);
    box.min.z = SDL_min(SDL_min(triangle->v1.z, triangle->v2.z), triangle->v3.z);

    box.max.x = SDL_max(SDL_max(triangle->v1.x, triangle->v2.x), triangle->v3.x);
    box.max.y = SDL_max(SDL_max(triangle->v1.y, triangle->v2.y), triangle->v3.y);
    box.max.z = SDL_max(SDL_max(triangle->v1.z, triangle->v2.z), triangle->v3.z);    

    return box;
}

// Helper function that computes the Axis-Aligned Bounding Box (AABB) that encapsulates all objects.
AABB computeObjectsAABB(Objects* objects)
{
    // If there are no objects in the scene, return a default AABB with zero values.
    if (objects->planeCount + objects->sphereCount + objects->triangleCount == 0)
    {
        return (AABB) {0};
    }

    AABB box;

    // Initialize the AABB with extreme values to ensure proper expansion.
    box.min.x = __FLT_MAX__;
    box.min.y = __FLT_MAX__;
    box.min.z = __FLT_MAX__;

    box.max.x = -__FLT_MAX__;
    box.max.y = -__FLT_MAX__;
    box.max.z = -__FLT_MAX__;

    int i;

    // Iterate through all planes and expand the bounding box accordingly.
    for (i = 0; i < objects->planeCount; i++)
    {
        Plane *plane = &objects->planes[i];
        AABB planeBox = computePlaneAABB(plane);

        // Update min and max bounds to include this plane's AABB.
        box.min.x = SDL_min(box.min.x, planeBox.min.x);
        box.min.y = SDL_min(box.min.y, planeBox.min.y);
        box.min.z = SDL_min(box.min.z, planeBox.min.z);

        box.max.x = SDL_max(box.max.x, planeBox.max.x);
        box.max.y = SDL_max(box.max.y, planeBox.max.y);
        box.max.z = SDL_max(box.max.z, planeBox.max.z);
    }

    // Iterate through all spheres and expand the bounding box accordingly.
    for (i = 0; i < objects->sphereCount; i++)
    {
        Sphere *sphere = &objects->spheres[i];
        AABB sphereBox = computeSphereAABB(sphere);

        // Update min and max bounds to include this sphere's AABB.
        box.min.x = SDL_min(box.min.x, sphereBox.min.x);
        box.min.y = SDL_min(box.min.y, sphereBox.min.y);
        box.min.z = SDL_min(box.min.z, sphereBox.min.z);

        box.max.x = SDL_max(box.max.x, sphereBox.max.x);
        box.max.y = SDL_max(box.max.y, sphereBox.max.y);
        box.max.z = SDL_max(box.max.z, sphereBox.max.z);
    }

    // Iterate through all triangles and expand the bounding box accordingly.
    for (i = 0; i < objects->triangleCount; i++)
    {
        Triangle *triangle = &objects->triangles[i];
        AABB triangleBox = computeTriangleAABB(triangle);

        // Update min and max bounds to include this triangle's AABB.
        box.min.x = SDL_min(box.min.x, triangleBox.min.x);
        box.min.y = SDL_min(box.min.y, triangleBox.min.y);
        box.min.z = SDL_min(box.min.z, triangleBox.min.z);

        box.max.x = SDL_max(box.max.x, triangleBox.max.x);
        box.max.y = SDL_max(box.max.y, triangleBox.max.y);
        box.max.z = SDL_max(box.max.z, triangleBox.max.z);
    }

    // Return the final computed bounding box that encapsulates all objects.
    return box;
}

// Comparison functions for sorting objects along different axes
int compareX(const void *a, const void *b)
{
    const struct ObjectArray *objA = (const struct ObjectArray *)a;
    const struct ObjectArray *objB = (const struct ObjectArray *)b;

    if (objA->center.x < objB->center.x) return -1;
    if (objA->center.x > objB->center.x) return 1;
    return 0;
}

int compareY(const void *a, const void *b)
{
    const struct ObjectArray *objA = (const struct ObjectArray *)a;
    const struct ObjectArray *objB = (const struct ObjectArray *)b;

    if (objA->center.y < objB->center.y) return -1;
    if (objA->center.y > objB->center.y) return 1;
    return 0;
}

int compareZ(const void *a, const void *b)
{
    const struct ObjectArray *objA = (const struct ObjectArray *)a;
    const struct ObjectArray *objB = (const struct ObjectArray *)b;

    if (objA->center.z < objB->center.z) return -1;
    if (objA->center.z > objB->center.z) return 1;
    return 0;
}

// Function to recursively build the BVH tree
BVHNode *buildBVH(Objects *objects)
{
    // Allocate memory for the BVH node
    BVHNode *node = malloc(sizeof(BVHNode));

    // Count total number of objects
    int numberOfObjects = objects->planeCount + objects->sphereCount + objects->triangleCount;
    if (numberOfObjects == 0)
    {
        return NULL; // If no objects, return NULL
    }

    // Base case: If the number of objects is small, store them in a leaf node
    if(numberOfObjects <= 2)
    {
        node->objects = *objects;
        node->left = NULL;
        node->right = NULL;
        node->bounds = computeObjectsAABB(objects); // Compute bounding box
        return node;
    }

    // Compute the bounding box of all objects
    AABB box = computeObjectsAABB(objects);
    node->bounds = box;

    // Determine the axis along which to split (longest axis)
    int axis = 0;
    float max = box.max.x - box.min.x;

    if(box.max.y - box.min.y > max)
    {
        axis = 1;
        max = box.max.y - box.min.y;
    }

    if(box.max.z - box.min.z > max)
    {
        axis = 2;
    }

    // Create an array to store objects with their centers for sorting
    struct ObjectArray *objectsArray = malloc(sizeof(struct ObjectArray) * numberOfObjects);
    int i;

    // Store planes in the array
    for(i = 0; i < objects->planeCount; i++)
    {
        Plane *plane = &objects->planes[i];
        objectsArray[i].center = plane->position;
        objectsArray[i].object = plane;
        objectsArray[i].type = 0;
    }

    // Store spheres in the array
    for(i = 0; i < objects->sphereCount; i++)
    {
        Sphere *sphere = &objects->spheres[i];
        objectsArray[i + objects->planeCount].center = sphere->position;
        objectsArray[i + objects->planeCount].object = sphere;
        objectsArray[i + objects->planeCount].type = 1;
    }

    // Store triangles in the array
    for(i = 0; i < objects->triangleCount; i++)
    {
        Triangle *triangle = &objects->triangles[i];
        objectsArray[i + objects->planeCount + objects->sphereCount].center = 
            multiplyVector(addVectors(triangle->v1, addVectors(triangle->v2, triangle->v3)), (1.0f / 3.0f));
        objectsArray[i + objects->planeCount + objects->sphereCount].object = triangle;
        objectsArray[i + objects->planeCount + objects->sphereCount].type = 2;
    }

    // Sort objects based on the selected axis
    if (axis == 0)
    {
        qsort(objectsArray, numberOfObjects, sizeof(struct ObjectArray), compareX);
    } 
    else if (axis == 1)
    {
        qsort(objectsArray, numberOfObjects, sizeof(struct ObjectArray), compareY);
    }
    else
    {
        qsort(objectsArray, numberOfObjects, sizeof(struct ObjectArray), compareZ);
    }

    // Determine the median index for splitting
    int medianIndex = numberOfObjects / 2;

    // Allocate and initialize the left objects list
    Objects leftObjects;
    leftObjects.maxPlanes = objects->maxPlanes;
    leftObjects.maxSpheres = objects->maxSpheres;
    leftObjects.maxTriangles = objects->maxTriangles;
    leftObjects.planes = malloc(sizeof(Plane) * objects->maxPlanes);
    leftObjects.spheres = malloc(sizeof(Sphere) * objects->maxSpheres);
    leftObjects.triangles = malloc(sizeof(Triangle) * objects->maxTriangles);
    leftObjects.planeCount = 0;
    leftObjects.sphereCount = 0;
    leftObjects.triangleCount = 0;

    // Assign objects to the left partition
    for(i = 0; i < medianIndex; i++)
    {
        if(objectsArray[i].type == 0)
        {
            leftObjects.planes[leftObjects.planeCount] = *(Plane *)objectsArray[i].object;
            leftObjects.planeCount++;
        }
        else if(objectsArray[i].type == 1)
        {
            leftObjects.spheres[leftObjects.sphereCount] = *(Sphere *)objectsArray[i].object;
            leftObjects.sphereCount++;
        }
        else
        {
            leftObjects.triangles[leftObjects.triangleCount] = *(Triangle *)objectsArray[i].object;
            leftObjects.triangleCount++;
        }
    }

    // Allocate and initialize the right objects list
    Objects rightObjects;
    rightObjects.maxPlanes = objects->maxPlanes;
    rightObjects.maxSpheres = objects->maxSpheres;
    rightObjects.maxTriangles = objects->maxTriangles;
    rightObjects.planes = malloc(sizeof(Plane) * objects->maxPlanes);
    rightObjects.spheres = malloc(sizeof(Sphere) * objects->maxSpheres);
    rightObjects.triangles = malloc(sizeof(Triangle) * objects->maxTriangles);
    rightObjects.planeCount = 0;
    rightObjects.sphereCount = 0;
    rightObjects.triangleCount = 0;

    // Assign objects to the right partition
    for(i = medianIndex; i < numberOfObjects; i++)
    {
        if(objectsArray[i].type == 0)
        {
            rightObjects.planes[rightObjects.planeCount] = *(Plane *)objectsArray[i].object;
            rightObjects.planeCount++;
        }
        else if(objectsArray[i].type == 1)
        {
            rightObjects.spheres[rightObjects.sphereCount] = *(Sphere *)objectsArray[i].object;
            rightObjects.sphereCount++;
        }
        else
        {
            rightObjects.triangles[rightObjects.triangleCount] = *(Triangle *)objectsArray[i].object;
            rightObjects.triangleCount++;
        }
    }

    // Free the temporary objects array
    free(objectsArray);

    // Recursively build left and right child nodes
    node->left = buildBVH(&leftObjects);
    node->right = buildBVH(&rightObjects);

    return node; // Return the constructed BVH node
}

float intersectAABB(Ray ray, AABB box)
{
    // Compute tMin and tMax for the X-axis
    float tMin = (box.min.x - ray.origin.x) / ray.direction.x;
    float tMax = (box.max.x - ray.origin.x) / ray.direction.x;

    // Ensure tMin is the smaller value
    if (tMin > tMax) 
    {
        float temp = tMin;
        tMin = tMax;
        tMax = temp;
    }

    // Compute tyMin and tyMax for the Y-axis
    float tyMin = (box.min.y - ray.origin.y) / ray.direction.y;
    float tyMax = (box.max.y - ray.origin.y) / ray.direction.y;

    // Ensure tyMin is the smaller value
    if (tyMin > tyMax)
    {
        float temp = tyMin;
        tyMin = tyMax;
        tyMax = temp;
    }

    // Check if the ray misses the box on the Y-axis
    if ((tMin > tyMax) || (tyMin > tMax))
        return 0.0f; // No intersection

    // Update tMin and tMax to account for Y-axis bounds
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    // Compute tzMin and tzMax for the Z-axis
    float tzMin = (box.min.z - ray.origin.z) / ray.direction.z;
    float tzMax = (box.max.z - ray.origin.z) / ray.direction.z;

    // Ensure tzMin is the smaller value
    if (tzMin > tzMax)
    {
        float temp = tzMin;
        tzMin = tzMax;
        tzMax = temp;
    }

    // Check if the ray misses the box on the Z-axis
    if ((tMin > tzMax) || (tzMin > tMax))
        return 0.0f; // No intersection

    // Return the closest valid intersection distance (ignoring negative values)
    return (tMin > 0.0f) ? tMin : tMax;
}

ObjectIntersection getClosestObjectIntersection(Ray ray, Objects* objects)
{
    float distance;
    float minDistance = __FLT_MAX__;
    ObjectIntersection closestIntersection;
    closestIntersection.objectType = -1;
    int objectIndex;
    bool intersectionFound = false;

    // Check for intersection with all spheres in the scene.
    for (int i = 0; i <objects->sphereCount; i++)
    {
        if (intersectRaySphere(ray, objects->spheres[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 0;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    // Check for intersection with all planes in the scene.
    for (int i = 0; i <objects->planeCount; i++)
    {
        if (intersectRayPlane(ray, objects->planes[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 1;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    // Check for intersection with all triangles in the scene.
    for (int i = 0; i < objects->triangleCount; i++)
    {
        if (intersectRayTriangle(ray, objects->triangles[i], &distance) && distance > 0 && distance < minDistance) 
        {
            minDistance = distance;
            closestIntersection.objectType = 2;
            objectIndex = i;
            intersectionFound = true;
        }
    }

    if (intersectionFound)
    {
        // Calculate the intersection point.
        closestIntersection.point = addVectors(ray.origin, multiplyVector(ray.direction, minDistance));
        
        // Determine the normal and material based on the object type.
        switch (closestIntersection.objectType)
        {
            case 0: // Sphere
                closestIntersection.normal = computeSphereNormal(closestIntersection.point,objects->spheres[objectIndex]);
                closestIntersection.material = objects->spheres[objectIndex].material;
                break;
            case 1: // Plane
                closestIntersection.normal = objects->planes[objectIndex].surfaceNormal;
                closestIntersection.material = objects->planes[objectIndex].material;
                break;
            case 2: // Triangle
                closestIntersection.normal = computeTriangleNormal(objects->triangles[objectIndex], ray.origin);
                closestIntersection.material = objects->triangles[objectIndex].material;
                break;
        }
    }
    else
    {
        // If no intersection is found, set default values.
        closestIntersection.point = (Vector){0, 0, 0};
        closestIntersection.normal = (Vector){0, 0, 0};
        closestIntersection.material = (Material){{0, 0, 0, 0}, 0, 0};
    }

    return closestIntersection;
}

int intersectBVH(Ray ray, BVHNode *node, ObjectIntersection *hit)
{
    // Check if the ray intersects the AABB of the current node
    float nodeIntersection = intersectAABB(ray, node->bounds);
    if (nodeIntersection <= 0.0f)
    {
        return 0; // No intersection with this node
    }

    // If this is a leaf node (no children), check for object intersections
    if (node->left == NULL && node->right == NULL)
    {
        *hit = getClosestObjectIntersection(ray, &node->objects);
        return 1; // Intersection found in the leaf node
    }

    // Check for intersection distances with the bounding boxes of the left and right child nodes
    float intersectDistanceLeft = node->left ? intersectAABB(ray, node->left->bounds) : 0.0f;
    float intersectDistanceRight = node->right ? intersectAABB(ray, node->right->bounds) : 0.0f;

    // Prioritize checking the child node with the closer intersection
    if (node->left != NULL && intersectDistanceLeft > 0.0f && 
        (intersectDistanceRight == 0.0f || intersectDistanceLeft < intersectDistanceRight))
    {
        if (intersectBVH(ray, node->left, hit))
        {
            return 1; // Intersection found in the left subtree
        }
    }

    if (node->right != NULL && intersectDistanceRight > 0.0f && 
        (intersectDistanceLeft == 0.0f || intersectDistanceRight < intersectDistanceLeft))
    {
        if (intersectBVH(ray, node->right, hit))
        {
            return 1; // Intersection found in the right subtree
        }
    }

    return 0; // No intersection found in this subtree
}

void freeBVH(BVHNode *node)
{
    if (node == NULL) {
        return;
    }

    // Recursively free left and right child nodes
    freeBVH(node->left);
    freeBVH(node->right);

    // Free the dynamically allocated objects' arrays
    free(node->objects.planes);
    free(node->objects.spheres);
    free(node->objects.triangles);

    // Free the BVH node itself
    free(node);
}
