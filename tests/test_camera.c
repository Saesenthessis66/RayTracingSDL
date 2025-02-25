#include "unity.h"
#include "camera.h"
#include <math.h>

// Tolerance for floating-point comparisons
#define EPSILON 0.0001
#define DIRECTION_EPSILON 0.002

void test_initCamera(void) {
    Camera cam;
    Vector pos = {0.0f, 0.0f, 0.0f};
    Vector dir = {0.0f, 0.0f, -1.0f};
    Vector up = {0.0f, 1.0f, 0.0f};
    float fov = 90.0f;
    int screenWidth = 800, screenHeight = 600;

    initCamera(&cam, pos, dir, up, fov, screenWidth, screenHeight);

    // Check camera properties
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.position.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.position.z);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.direction.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.direction.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -1.0f, cam.direction.z);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.upVector.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, cam.upVector.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cam.upVector.z);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 90.0f, cam.fieldOfView);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, (float)screenWidth / screenHeight, cam.aspectRatio);
}

void test_mapPixelToRay(void) {
    Camera cam;
    Vector pos = {0.0f, 0.0f, 0.0f};
    Vector dir = {0.0f, 0.0f, -1.0f};
    Vector up = {0.0f, 1.0f, 0.0f};
    float fov = 90.0f;
    int screenWidth = 1600, screenHeight = 1200;

    initCamera(&cam, pos, dir, up, fov, screenWidth, screenHeight);

    // Test ray from screen center
    Ray centerRay = mapPixelToRay(&cam, screenWidth / 2, screenHeight / 2, screenWidth, screenHeight);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, centerRay.origin.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, centerRay.origin.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, centerRay.origin.z);
    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, 0.0f, centerRay.direction.x);
    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, 0.0f, centerRay.direction.y);
    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, -1.0f, centerRay.direction.z);
    
    // Test ray from top-left corner
    Ray cornerRay = mapPixelToRay(&cam, 0, 0, screenWidth, screenHeight);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cornerRay.origin.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cornerRay.origin.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cornerRay.origin.z);
    
    // Expected direction for top-left corner
    float expectedX = -cam.aspectRatio * SDL_tan((fov * SDL_PI_F / 180.0f) / 2);
    float expectedY = SDL_tan((fov * SDL_PI_F / 180.0f) / 2);
    float expectedZ = -1.0f;

    // Normalize the expected direction for accuracy
    Vector expectedDir = { expectedX, expectedY, expectedZ };
    expectedDir = normalizeVector(expectedDir);

    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, expectedDir.x, cornerRay.direction.x);
    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, expectedDir.y, cornerRay.direction.y);
    TEST_ASSERT_FLOAT_WITHIN(DIRECTION_EPSILON, expectedDir.z, cornerRay.direction.z);
}

