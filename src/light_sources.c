#include "light_sources.h"

float computePointLightIntensity(PointLight *light, Vector point)
{
    // Compute vector from light source to the point
    Vector lightToPoint = subtractVectors(point, light->position);
    
    // Compute the distance from the light to the point
    float distance = vectorLength(lightToPoint);

    // If the point is outside the light's range, return 0 intensity
    if (distance > light->range) return 0.0f;

    // Apply attenuation using the inverse square law with a small offset to prevent extreme values
    float attenuation = 1.0f / (0.01f + distance * distance);  

    // Compute final intensity and clamp the result between 0 and 1
    float intensity = light->material.intensity * attenuation;
    return SDL_clamp(intensity, 0.0f, 1.0f);
}

float computeDirectionalLightIntensity(DirectionalLight *light, Vector normal)
{
    // Compute the influence of the light on the surface using the dot product
    // The dot product measures how aligned the surface normal is with the light direction
    float alignment = dotProduct(normal, normalizeVector(light->direction));

    // If the alignment is not negative, the surface is facing away from the light, so return 0 intensity
    if (alignment >= 0) return 0.0f;

    // Scale the light intensity based on the inversed alignement and clamp it between 0 and 1
    return SDL_clamp(light->material.intensity * alignment * -1.0f, 0.0f, 1.0f);
}

float computeSpotLightIntensity(SpotLight *light, Vector point, Vector normal)
{
    // Compute the normalized vector from the light source to the point
    Vector directionToPoint = normalizeVector(subtractVectors(point, light->position));

    // Compute the cosine of the angle between the spotlight direction and the direction to the point
    float angleCosine = dotProduct(light->direction, directionToPoint);

    // Convert cutoff angles from degrees to cosine space
    float outerCutoffCosine = SDL_cosf(light->cutoffAngle * SDL_PI_F / 180.0);
    float innerCutoffCosine = SDL_cosf(light->innerCutoffAngle * SDL_PI_F / 180.0);

    // If the point falls outside the outer cutoff angle, there is no illumination
    if (angleCosine < outerCutoffCosine) return 0.0f;

    // If the point is inside the inner cutoff angle, full intensity is applied
    if (angleCosine > innerCutoffCosine) return light->material.intensity;

    // Compute the smooth interpolation factor between the inner and outer cutoff
    float t = (angleCosine - innerCutoffCosine) / (outerCutoffCosine - innerCutoffCosine);
    t = SDL_clamp(t, 0.0f, 1.0f); // Clamp to valid range

    // Apply smooth transition from full intensity to zero based on the interpolation factor
    return (1.0f - t) * light->material.intensity;
}

float computeAmbientLightIntensity(AmbientLight *light)
{
    // Return the intensity of the ambient light
    return light->material.intensity;
}