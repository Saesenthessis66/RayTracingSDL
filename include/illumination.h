#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include "scene.h"
#include <SDL3/SDL.h>

// Computes the diffuse lighting contribution from a point light source at a given point
float computePointLightDiffuse(PointLight* light, Vector point, Vector normal, Scene* scene);

// Computes the diffuse lighting contribution from a directional light source
float computeDirectionalLightDiffuse(DirectionalLight* light, Vector point, Vector normal, Scene* scene);

// Computes the diffuse lighting contribution from a spotlight at a given point
float computeSpotLightDiffuse(SpotLight* light, Vector point, Vector normal, Scene* scene);

// Computes the specular lighting contribution from a point light source
float computePointLightSpecular(PointLight* light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene);

// Computes the specular lighting contribution from a directional light source
float computeDirectionalLightSpecular(DirectionalLight* light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene);

// Computes the specular lighting contribution from a spotlight
float computeSpotLightSpecular(SpotLight* light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene);

// Computes the final surface color at a given point based on lighting and material properties
SDL_Color computeSurfaceColor(Scene* scene, Vector point, Vector normal, Vector viewDirection, Material material);

#endif // ILLUMINATION_H