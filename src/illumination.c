#include "illumination.h"

float computePointLightDiffuse(PointLight *light, Vector point, Vector normal, Scene *scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadow(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computePointLightIntensity(light, point);

    // If intensity is zero or negative, return immediately (no contribution)
    if (lightIntensity <= 0.0f) return 0.0f;

    // Compute the normalized direction from the point to the light source
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));

    // Compute the diffuse factor using Lambert's cosine law (dot product of normal and light direction)
    float diffuseFactor = dotProduct(normal, lightDirection);

    // If the surface is facing away from the light, there is no diffuse lighting
    if (diffuseFactor <= 0.0f) return 0.0f;

    // Compute the final diffuse contribution, clamping the result between 0 and 1
    float diffuseLight = SDL_clamp(diffuseFactor, 0.0f, 1.0f) * lightIntensity;

    return diffuseLight;
}

float computeDirectionalLightDiffuse(DirectionalLight *light, Vector point, Vector normal, Scene* scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadowDir(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computeDirectionalLightIntensity(light, normal);

    // Compute the diffuse factor using Lambert's cosine law
    float diffuseFactor = dotProduct(normal, light->direction);

    // If the surface is facing away from the light, there is no diffuse lighting
    if (diffuseFactor <= 0.0f) return 0.0f;

    // Compute final diffuse contribution
    return diffuseFactor * lightIntensity;
}

float computeSpotLightDiffuse(SpotLight *light, Vector point, Vector normal, Scene* scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadowSpot(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computeSpotLightIntensity(light, point, normal);

    // If intensity is zero or negative, return immediately (no contribution)
    if (lightIntensity <= 0.0f) return 0.0f;


    // Compute the diffuse factor using Lambert's cosine law (dot product of normal and light direction)
    float diffuseFactor = dotProduct(normal, light->direction);

    // If the surface is facing away from the light, there is no diffuse lighting
    if (diffuseFactor <= 0.0f) return 0.0f;

    // Compute the final diffuse contribution, clamping the result between 0 and 1
    float diffuseLight = SDL_clamp(diffuseFactor, 0.0f, 1.0f) * lightIntensity;

    return diffuseLight;
}

float computePointLightSpecular(PointLight *light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadow(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computePointLightIntensity(light, point);

    // If intensity is zero or negative, return immediately (no contribution)
    if (lightIntensity <= 0.0f) return 0.0f;

    // Compute the normalized direction from the point to the light source
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));

    // Compute the reflection vector using the formula: R = 2 * (N . L) * N - L
    float normalDotLight = dotProduct(normal, lightDirection);
    Vector reflectionVector = subtractVectors(multiplyVector(normal, 2.0f * normalDotLight), lightDirection);

    // Compute the specular component using the view direction
    float specularFactor = dotProduct(reflectionVector, viewDirection);

    // If specular factor is negative, return 0 (no highlight)
    if (specularFactor <= 0.0f) return 0.0f;

    // Apply shininess exponent
    float specular = SDL_powf(specularFactor, shininess);

    // Compute the final specular contribution, clamping between 0 and 1
    float specularLight = SDL_clamp(specular * lightIntensity, 0.0f, 1.0f);
    
    return specularLight;
}

float computeDirectionalLightSpecular(DirectionalLight *light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadowDir(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computeDirectionalLightIntensity(light, point);

    // If intensity is zero or negative, return immediately (no contribution)
    if (lightIntensity <= 0.0f) return 0.0f;

    // Compute the reflection vector using the formula: R = 2 * (N . L) * N - L
    float normalDotLight = dotProduct(normal, light->direction);
    Vector reflectionVector = subtractVectors(multiplyVector(normal, 2.0f * normalDotLight), light->direction);

    // Compute the specular component using the view direction
    float specularFactor = dotProduct(reflectionVector, viewDirection);

    // If specular factor is negative, return 0 (no highlight)
    if (specularFactor <= 0.0f) return 0.0f;

    // Apply shininess exponent
    float specular = SDL_powf(specularFactor, shininess);

    // Compute the final specular contribution, clamping between 0 and 1
    float specularLight = SDL_clamp(specular * lightIntensity, 0.0f, 1.0f);
    
    return specularLight;
}

float computeSpotLightSpecular(SpotLight *light, Vector point, Vector normal, Vector viewDirection, float shininess, Scene* scene)
{
    // Check if the point is in shadow from this light source
    if (isPointInShadowSpot(point, light, scene)) return 0.0f;

    // Compute light intensity at the given point
    float lightIntensity = computeSpotLightIntensity(light, point, normal);

    // If intensity is zero or negative, return immediately (no contribution)
    if (lightIntensity <= 0.0f) return 0.0f;

    // Compute the normalized direction from the point to the light source
    Vector lightDirection = normalizeVector(subtractVectors(light->position, point));

    // Compute the reflection vector using the formula: R = 2 * (N . L) * N - L
    float normalDotLight = dotProduct(normal, lightDirection);
    Vector reflectionVector = subtractVectors(multiplyVector(normal, 2.0f * normalDotLight), lightDirection);

    // Compute the specular component using the view direction
    float specularFactor = dotProduct(reflectionVector, viewDirection);

    // If specular factor is negative, return 0 (no highlight)
    if (specularFactor <= 0.0f) return 0.0f;

    // Apply shininess exponent
    float specular = SDL_powf(specularFactor, shininess);

    // Compute the final specular contribution, clamping between 0 and 1
    float specularLight = SDL_clamp(specular * lightIntensity, 0.0f, 1.0f);
    
    return specularLight;
}

SDL_Color computeSurfaceColor(Scene *scene, Vector point, Vector normal, Vector viewDirection, Material material)
{
    // Final computed color values
    float finalRed = 0.0f, finalGreen = 0.0f, finalBlue = 0.0f;
    float diffuseRed = 0.0f, diffuseGreen = 0.0f, diffuseBlue = 0.0f;
    float specularRed = 0.0f, specularGreen = 0.0f, specularBlue = 0.0f;

    // Extract ambient lighting properties
    float ambientIntensity = scene->lights.ambientLight.material.intensity;
    float ambientRed = (scene->lights.ambientLight.material.color.r / 255.0f) * ambientIntensity;
    float ambientGreen = (scene->lights.ambientLight.material.color.g / 255.0f) * ambientIntensity;
    float ambientBlue = (scene->lights.ambientLight.material.color.b / 255.0f) * ambientIntensity;

    // Process each point light
    for (int i = 0; i < scene->lights.pointLightCount; i++)
    {
        float diffuse = computePointLightDiffuse(&scene->lights.pointLights[i], point, normal, scene);
        float specular = computePointLightSpecular(&scene->lights.pointLights[i], point, normal, viewDirection, material.shininess, scene);

        float lightRed = scene->lights.pointLights[i].material.color.r / 255.0f;
        float lightGreen = scene->lights.pointLights[i].material.color.g / 255.0f;
        float lightBlue = scene->lights.pointLights[i].material.color.b / 255.0f;

        diffuseRed += lightRed * diffuse;
        diffuseGreen += lightGreen * diffuse;
        diffuseBlue += lightBlue * diffuse;

        specularRed += lightRed * specular;
        specularGreen += lightGreen * specular;
        specularBlue += lightBlue * specular;
    }

    // Process each directional light
    for (int i = 0; i < scene->lights.directionalLightCount; i++)
    {
        float diffuse = computeDirectionalLightDiffuse(&scene->lights.directionalLights[i], point, normal, scene);
        float specular = computeDirectionalLightSpecular(&scene->lights.directionalLights[i], point, normal, viewDirection, material.shininess, scene);

        float lightRed = scene->lights.directionalLights[i].material.color.r / 255.0f;
        float lightGreen = scene->lights.directionalLights[i].material.color.g / 255.0f;
        float lightBlue = scene->lights.directionalLights[i].material.color.b / 255.0f;

        diffuseRed += lightRed * diffuse;
        diffuseGreen += lightGreen * diffuse;
        diffuseBlue += lightBlue * diffuse;

        specularRed += lightRed * specular;
        specularGreen += lightGreen * specular;
        specularBlue += lightBlue * specular;
    }

    // Process each spotlight
    for (int i = 0; i < scene->lights.spotLightCount; i++)
    {
        float diffuse = computeSpotLightDiffuse(&scene->lights.spotLights[i], point, normal, scene);
        float specular = computeSpotLightSpecular(&scene->lights.spotLights[i], point, normal, viewDirection, material.shininess, scene);

        float lightRed = scene->lights.spotLights[i].material.color.r / 255.0f;
        float lightGreen = scene->lights.spotLights[i].material.color.g / 255.0f;
        float lightBlue = scene->lights.spotLights[i].material.color.b / 255.0f;

        diffuseRed += lightRed * diffuse;
        diffuseGreen += lightGreen * diffuse;
        diffuseBlue += lightBlue * diffuse;

        specularRed += lightRed * specular;
        specularGreen += lightGreen * specular;
        specularBlue += lightBlue * specular;
    }

    // Clamp diffuse and specular values
    specularRed = SDL_clamp(specularRed, 0.0f, 1.0f);
    specularGreen = SDL_clamp(specularGreen, 0.0f, 1.0f);
    specularBlue = SDL_clamp(specularBlue, 0.0f, 1.0f);

    diffuseRed = SDL_clamp(diffuseRed, 0.0f, 1.0f);
    diffuseGreen = SDL_clamp(diffuseGreen, 0.0f, 1.0f);
    diffuseBlue = SDL_clamp(diffuseBlue, 0.0f, 1.0f);

    // Compute final color, ensuring material color affects diffuse component
    finalRed = ambientRed + (material.color.r / 255.0f) * diffuseRed + specularRed;
    finalGreen = ambientGreen + (material.color.g / 255.0f) * diffuseGreen + specularGreen;
    finalBlue = ambientBlue + (material.color.b / 255.0f) * diffuseBlue + specularBlue;

    // Clamp the final color values before converting to SDL_Color
    SDL_Color resultColor = {0}; // Initialize the color struct
    resultColor.a = material.color.a;
    resultColor.r = (Uint8)SDL_clamp(finalRed * 255.0f, 0, 255);
    resultColor.g = (Uint8)SDL_clamp(finalGreen * 255.0f, 0, 255);
    resultColor.b = (Uint8)SDL_clamp(finalBlue * 255.0f, 0, 255);

    return resultColor;
}