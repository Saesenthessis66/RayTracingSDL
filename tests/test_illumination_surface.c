#include "unity.h"
#include "scene.h"
#include "illumination.h"

#define EPSILON 0.0001

bool areColorsEqual(SDL_Color c1, SDL_Color c2)
{
    return abs(c1.r - c2.r) <= EPSILON &&
           abs(c1.g - c2.g) <= EPSILON &&
           abs(c1.b - c2.b) <= EPSILON &&
           c1.a == c2.a;
}

// Tests surface color computation when there are no active lights in the scene.
void test_ComputeSurfaceColor_NoLights(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};
    viewDir = (Vector){0, 0, 1};
    testMat = (Material){(SDL_Color){255, 0, 0, 255}, 0.5f, 8.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    SDL_Color expectedColor = (SDL_Color){0, 0, 0, 255};
    
    TEST_ASSERT_TRUE(areColorsEqual(result, expectedColor));
}

// Tests surface color computation when multiple light sources contribute to the final color.
void test_ComputeSurfaceColor_MultipleLights(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    Vector lightPos = {0, 1, -1};
    Vector lightDir = {0, 1, -1};
    LightMaterial lightMat = {(SDL_Color){128, 255, 0, 255}, 0.5f};
    LightMaterial ambientMat = {(SDL_Color){255, 255, 255, 255}, 0.5f};

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    setAmbientLight(&testScene, ambientMat);
    addPointLight(&testScene, lightMat, lightPos, 10.0f);
    addDirectionalLight(&testScene, lightMat, lightPos, lightDir);
    addSpotLight(&testScene, lightMat, lightPos, lightDir, 40.0f, 20.0f);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};
    viewDir = (Vector){0, 1, -1};
    testMat = (Material){(SDL_Color){255, 128, 0, 255}, 0.5f, 8.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(127, result.r);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(127, result.g);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(127, result.b); 
}

// Tests surface color computation for a highly reflective material with maximum shininess.
void test_ComputeSurfaceColor_HighShininess(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    Vector lightPos = {0, 1, -1};
    Vector lightDir = {0, 1, -1};
    LightMaterial lightMat = {(SDL_Color){128, 255, 0, 255}, 0.5f};

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    addDirectionalLight(&testScene, lightMat, lightPos, lightDir);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};
    viewDir = (Vector){0, 1, 1};
    testMat = (Material){(SDL_Color){255, 128, 0, 255}, 0.5f, 64.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(127, result.r);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(200, result.g);
    TEST_ASSERT_EQUAL_UINT8(0, result.b);
}

// Tests surface color computation for a material with low shininess, ensuring a soft specular highlight.
void test_ComputeSurfaceColor_LowShininess(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    Vector lightPos = {0, 1, -1};
    Vector lightDir = {0, 1, -1};
    LightMaterial lightMat = {(SDL_Color){128, 255, 0, 255}, 0.5f};

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    addDirectionalLight(&testScene, lightMat, lightPos, lightDir);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};
    viewDir = (Vector){0, 1, 1};
    testMat = (Material){(SDL_Color){255, 128, 0, 255}, 0.5f, 2.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(100, result.r);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(200, result.g);
    TEST_ASSERT_EQUAL_UINT8(0, result.b);
}

// Tests that a surface facing away from the light does not receive direct illumination.
void test_ComputeSurfaceColor_LightOppositeNormal(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    Vector lightPos = {0, 1, -1};
    Vector lightDir = {0, 1, -1};
    LightMaterial lightMat = {(SDL_Color){128, 255, 0, 255}, 0.5f};

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    addDirectionalLight(&testScene, lightMat, lightPos, lightDir);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, -1, 0};
    viewDir = (Vector){0, 1, 1};
    testMat = (Material){(SDL_Color){255, 128, 0, 255}, 0.5f, 2.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    TEST_ASSERT_EQUAL_UINT8(0, result.r);
    TEST_ASSERT_EQUAL_UINT8(0, result.g);
    TEST_ASSERT_EQUAL_UINT8(0, result.b);
}

// Tests that the final computed color does not exceed the maximum RGB range when multiple strong lights are present.
void test_ComputeSurfaceColor_MaxIntensityClamping(void)
{
    Scene testScene;
    Vector testPoint;
    Vector testNormal;
    Vector viewDir;
    Material testMat;

    Vector lightPos = {0, 1, -1};
    Vector lightDir = {0, 1, -1};
    LightMaterial lightMat = {(SDL_Color){200, 200, 200, 255}, 0.5f};
    LightMaterial ambientMat = {(SDL_Color){255, 255, 255, 255}, 0.5f};

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    setAmbientLight(&testScene, ambientMat);
    addPointLight(&testScene, lightMat, lightPos, 10.0f);
    addDirectionalLight(&testScene, lightMat, lightPos, lightDir);
    addSpotLight(&testScene, lightMat, lightPos, lightDir, 40.0f, 20.0f);

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};
    viewDir = (Vector){0, 1, 1};
    testMat = (Material){(SDL_Color){200, 200, 200, 255}, 1.0f, 16.0f};

    SDL_Color result = computeSurfaceColor(&testScene, testPoint, testNormal, viewDir, testMat);

    TEST_ASSERT_EQUAL_UINT8(255, result.r);
    TEST_ASSERT_EQUAL_UINT8(255, result.g);
    TEST_ASSERT_EQUAL_UINT8(255, result.b); 
}
