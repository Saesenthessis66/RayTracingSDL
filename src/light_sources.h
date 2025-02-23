#ifndef LIGHT_SOURCES_H
#define LIGHT_SOURCES_H

#include "ray.h"  

// Structure representing the material properties of a light source (color and intensity)
typedef struct {
    SDL_Color color;    // The color of the light (RGBA format)
    float intensity;    // The intensity of the light (brightness)
} LightMaterial;

// Structure representing a point light source in 3D space
typedef struct {
    Vector position;    // Position of the point light in world space
    float range;        // Range of the point light (how far it affects the scene)
    LightMaterial material; // Material properties of the light (color and intensity)
} PointLight;

    // Structure representing a directional light source (light coming from a specific direction)
    typedef struct {
        Vector position;    // Position of the light (can be considered as the light's origin)
        Vector direction;   // Direction of the light (the light is emitted along this vector)
        LightMaterial material; // Material properties of the light
    } DirectionalLight;

// Structure representing a spotlight (a cone-shaped light source)
typedef struct {
    Vector position;    // Position of the spotlight
    Vector direction;   // Direction the spotlight is facing
    float cutoffAngle;      // Outer cutoff angle (hard limit)
    float innerCutoffAngle; // Inner cutoff angle (full brightness inside this)
    LightMaterial material; // Material properties of the light
} SpotLight;

// Structure representing ambient light (light that affects the entire scene equally)
typedef struct {
    LightMaterial material;  // Material properties of the ambient light (color and intensity)
} AmbientLight;

// Structure that holds all light sources in the scene (point lights, directional lights, spotlights, and ambient light)
typedef struct {
    PointLight* pointLights;           // Array of point light sources
    DirectionalLight* directionalLights; // Array of directional light sources
    SpotLight* spotLights;             // Array of spotlight sources
    AmbientLight ambientLight;         // Ambient light that affects the entire scene
    int pointLightCount;               // Count of point light sources
    int directionalLightCount;         // Count of directional light sources
    int spotLightCount;                // Count of spotlight sources
    int maxPointLights;                // Max amount of point lights
    int maxDirectionalLights;          // Max amount of directional lights
    int maxSpotLights;                 // Max amount of spot lights
} SceneLights;

// Function to compute the intensity of a point light at a given point in space
float computePointLightIntensity(PointLight* light, Vector point);

// Function to compute the intensity of a directional light at a given normal (direction of surface)
float computeDirectionalLightIntensity(DirectionalLight* light, Vector normal);

// Function to compute the intensity of a spotlight at a given point and normal (direction of surface)
float computeSpotLightIntensity(SpotLight* light, Vector point, Vector normal);

// Function to compute the intensity of the ambient light for the scene
float computeAmbientLightIntensity(AmbientLight* light);

#endif // LIGHT_SOURCES_H