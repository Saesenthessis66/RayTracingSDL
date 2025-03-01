#include "unity.h"
#include "scene.h"
#include "illumination.h"

// Test case: Point is in shadow due to an object blocking the light
void test_ComputePointLightDiffuse_PointInShadow(void) {
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Plane shadowCastingPlane;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    Material planeMaterial = (Material){{255, 255, 255, 255}, 0.5f, 0.5f};
    Vector planePosition = (Vector){0, 1, 0};
    Vector planeNormal = (Vector){0, 1, 0};

    addPlane(&testScene,planePosition, planeNormal, 5, 5, planeMaterial);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector pointLightPosition = (Vector){0, 2, 0};

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};

    float result = computePointLightDiffuse(&testLight, testPoint, planeNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Light intensity is zero, should return 0.0f
void test_ComputePointLightDiffuse_ZeroLightIntensity(void) {
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector pointLightPosition = (Vector){0, 6, 0};

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};  

    float result = computePointLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Normal facing away from the light
void test_ComputePointLightDiffuse_NormalFacingAway(void) {
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector pointLightPosition = (Vector){0, 2, 0};

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, -1, 0};

    float result = computePointLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Valid diffuse contribution (light hitting surface)
void test_ComputePointLightDiffuse_ValidDiffuse(void) {
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector pointLightPosition = {0, 5, 0};

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};

    float result = computePointLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, result);    
}