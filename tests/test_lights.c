#include "unity.h"
#include "light_sources.h"

#define EPSILON 0.0001

void test_computePointLightIntensity(void) {
    PointLight light = {{0.0f, 5.0f, 0.0f}, 10.0f, {{255, 255, 255, 255}, 1.0f}};
    Vector point = {0.0f, 0.0f, 0.0f};

    float expected_intensity = 1.0f / (0.01f + 25.0f); // distance = 5, so distance squared = 25
    expected_intensity = SDL_clamp(expected_intensity, 0.0f, 1.0f);

    float intensity = computePointLightIntensity(&light, point);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected_intensity, intensity);
}


void test_computeDirectionalLightIntensity(void) {
    DirectionalLight light = {{0.0f, 10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {{255, 255, 255, 255}, 1.0f}};
    Vector normal = {0.0f, 1.0f, 0.0f};

    float intensity = computeDirectionalLightIntensity(&light, normal);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, intensity);
}

void test_computeSpotLightIntensity(void) {
    SpotLight light = {{0.0f, 5.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 30.0f, 15.0f, {{255, 255, 255, 255}, 1.0f}};
    Vector point = {0.0f, 0.0f, 0.0f};
    Vector normal = {0.0f, 1.0f, 0.0f};

    float intensity = computeSpotLightIntensity(&light, point, normal);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, intensity);
}

void test_computeAmbientLightIntensity(void) {
    AmbientLight light = {{{255, 255, 255, 255}, 0.5f}};

    float intensity = computeAmbientLightIntensity(&light);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.5f, intensity);
}
