#include <SDL3/SDL.h>

#include <stdio.h>

// Structure representing a 3D vector
typedef struct{
    float x, y, z;
} Vector;

// Structure representing a ray with an origin and a direction
typedef struct {
    Vector origin; 
    Vector direction;
} Ray;

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

// Function to compute the cross product of two vectors (useful for generating orthogonal directions)
Vector vectorCrossProduct(Vector u, Vector v)
{
    Vector cross;
    cross.x = u.y * v.z - u.z * v.y;
    cross.y = u.z * v.x - u.x * v.z;
    cross.z = u.x * v.y - u.y * v.x;
    return cross;
}