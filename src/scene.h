#ifndef SCENE_H
#define SCENE_H

#include "shapes.h"  // Include the shapes header file to use Objects, Sphere, Plane, Triangle, and Material types
#include "light_sources.h"

// Structure representing a scene with various objects
typedef struct {
    Objects objects;  // Objects contained in the scene (spheres, planes, triangles)
    SceneLights lights; // Lights contained in the scene (point lights, directional lights, spotlights, and ambient light)
} Scene;

// Function to initialize a scene with dynamic memory allocation for spheres, planes, and triangles
void initScene(Scene* scene, int maxSpheres, int maxPlanes, int maxTriangles, int maxPointLights, int maxDirectionalLights, int maxSpotLights);

// Function to free memory allocated for the scene's objects
void freeScene(Scene* scene);

// Function to add a sphere to the scene
void addSphere(Scene* scene, Vector position, float radius, Material material, int maxSpheres);

// Function to add a plane to the scene
void addPlane(Scene* scene, Vector position, Vector vector, Material material, int maxPlanes);

// Function to add a triangle to the scene
void addTriangle(Scene* scene, Vector v1, Vector v2, Vector v3, Material material, int maxTriangles);

// Function to add a point light
void addPointLight(Scene* scene, LightMaterial material, Vector position, float range, int maxPointLights);

// Function to add a directional light
void addDirectionalLight(Scene* scene, LightMaterial material, Vector position, Vector direction, int maxDirectionalLights);

// Function to add a spotlight
void addSpotLight(Scene* scene, LightMaterial material, Vector position, Vector direction, float cutOffAngle, int maxSpotLights);

// Function to set ambient light
void setAmbientLight(Scene* scene, LightMaterial material);

// Function to check if a point is in shadow relative to a point light source
int isPointInShadow(Vector point, PointLight* light, Scene* scene);

// Function to check if a point is in shadow relative to a directional light source
int isPointInShadowDir(Vector point, DirectionalLight* light, Scene* scene);

// Function to check if a point is in shadow relative to a spotlight source
int isPointInShadowSpot(Vector point, SpotLight* light, Scene* scene);

#endif // SCENE_H