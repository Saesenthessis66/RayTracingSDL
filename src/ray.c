#include "ray.h"

#include <stdio.h>
#include <stdlib.h>

// Function to compute the length (magnitude) of a vector
float vectorLength(Vector v)
{
    return SDL_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Function to multiply a vector by a scalar
Vector multiplyVector(Vector v, float multiplier)
{
    v.x *= multiplier;
    v.y *= multiplier;
    v.z *= multiplier;
    return v;
}

// Function to add two vectors
Vector addVectors(Vector u, Vector v)
{
    u.x += v.x;
    u.y += v.y;
    u.z += v.z;
    return u;
}

// Function to subtract two vectors
Vector subtractVectors(Vector u, Vector v)
{
    u.x -= v.x;
    u.y -= v.y;
    u.z -= v.z;
    return u;
}

// Function to normalize a vector (make its length = 1)
Vector normalizeVector(Vector v) {
    float l = vectorLength(v);
    if (l == 0) { 
        printf("Cannot normalize a zero vector!\n");
        return v;  // Return unchanged vector if it's zero-length to avoid division by zero
    }
    Vector result = {v.x / l, v.y / l, v.z / l};
    return result;
}

// Function to compute the cross product of two vectors
Vector vectorCrossProduct(Vector u, Vector v)
{
    Vector cross;
    cross.x = u.y * v.z - u.z * v.y;
    cross.y = u.z * v.x - u.x * v.z;
    cross.z = u.x * v.y - u.y * v.x;
    return cross;
}

// Function to compute dot product of two vectors
float dotProduct(Vector u, Vector v)
{
    float ret = u.x * v.x + u.y * v.y + u.z * v.z;
}

// Function to print vector
void printVector(Vector v)
{
    printf("Vector: (%.2f, %.2f, %.2f)\n", v.x, v.y, v.z);
}