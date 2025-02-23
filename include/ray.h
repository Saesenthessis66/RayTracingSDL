#ifndef RAY_H
#define RAY_H

#include <SDL3/SDL.h>

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
float vectorLength(Vector v);

// Function to multiply a vector by a scalar
Vector multiplyVector(Vector v, float multiplier);

// Function to add two vectors
Vector addVectors(Vector u, Vector v);

// Function to subtract two vectors
Vector subtractVectors(Vector u, Vector v);

// Function to normalize a vector (make its length = 1)
Vector normalizeVector(Vector v);

// Function to compute the cross product of two vectors
Vector vectorCrossProduct(Vector u, Vector v);

// Function to compute dot product of two vectors
float dotProduct(Vector u, Vector v);

// Function to print vector
void printVector(Vector v);

#endif // RAY_H