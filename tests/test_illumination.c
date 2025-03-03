#include "unity.h"
#include "scene.h"
#include "illumination.h"

// Test case: Point is in shadow due to an object blocking the point light
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

// Test case: Point Light intensity is zero, should return 0.0f
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

// Test case: Normal facing away from the point light
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

// Test case: Valid diffuse contribution (point light hitting surface)
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

// Test case: Point is in shadow due to an object blocking the directional light
void test_ComputeDirectionalLightDiffuse_PointInShadow(void) {
    Scene testScene;
    DirectionalLight testLight;
    Vector testPoint;
    Plane shadowCastingPlane;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    Material planeMaterial = (Material){{255, 255, 255, 255}, 0.5f, 0.5f};
    Vector planePosition = (Vector){0, 1, 0};
    Vector planeNormal = (Vector){0, 1, 0};

    addPlane(&testScene,planePosition, planeNormal, 5, 5, planeMaterial);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (DirectionalLight){(Vector){0, 0, 0}, lightDirection, lightMaterial};

    testPoint = (Vector){0, 0, 0};

    float result = computeDirectionalLightDiffuse(&testLight, testPoint, planeNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Normal facing away from the directional light
void test_ComputeDirectionalLightDiffuse_NormalFacingAway(void) {
    Scene testScene;
    DirectionalLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (DirectionalLight){(Vector){0, 0, 0}, lightDirection, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, -1, 0};

    float result = computeDirectionalLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Valid diffuse contribution (directional light hitting surface)
void test_ComputeDirectionalLightDiffuse_ValidDiffuse(void) {
    Scene testScene;
    DirectionalLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (DirectionalLight){(Vector){0, 0, 0}, lightDirection, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};

    float result = computeDirectionalLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, result);    
}

// Test case: Point is in shadow due to an object blocking the spot light
void test_ComputeSpotLightDiffuse_PointInShadow(void) {
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Plane shadowCastingPlane;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    Material planeMaterial = (Material){{255, 255, 255, 255}, 0.5f, 0.5f};
    Vector planePosition = (Vector){0, 1, 0};
    Vector planeNormal = (Vector){0, 1, 0};

    addPlane(&testScene, planePosition, planeNormal, 5, 5, planeMaterial);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (SpotLight){(Vector){0, 5, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0, 0, 0};

    float result = computeSpotLightDiffuse(&testLight, testPoint, planeNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Spot Light intensity is zero, should return 0.0f
void test_ComputeSpotLightDiffuse_ZeroLightIntensity(void) {
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (SpotLight){(Vector){50, 5, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};  

    float result = computeSpotLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Normal facing away from the spot light
void test_ComputeSpotLightDiffuse_NormalFacingAway(void) {
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (SpotLight){(Vector){0, 5, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, -1, 0};

    float result = computeSpotLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Valid diffuse contribution (spot light hitting surface)
void test_ComputeSpotLightDiffuse_ValidDiffuse(void) {
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector lightDirection = normalizeVector((Vector){1, 1, 0});

    testLight = (SpotLight){(Vector){0.8f, 1, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = (Vector){0, 1, 0};

    float result = computeSpotLightDiffuse(&testLight, testPoint, testNormal, &testScene);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, result);    
}

// Test case: Point in shadow, no specular reflection expected
void test_ComputePointLightSpecular_PointInShadow(void)
{
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

    Vector viewDir = (Vector){0, 0, 1};

    float result = computePointLightSpecular(&testLight, testPoint, planeNormal, viewDir, 1.0f, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

void test_ComputePointLightSpecular_PerfectReflection(void)
{
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Vector testNormal = (Vector){0, 1, 0}; // Upward-facing surface

    // Initialize scene
    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f}; // Full intensity
    Vector pointLightPosition = (Vector){0, 2, 2}; // Slightly angled light

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};

    Vector viewDir = normalizeVector((Vector){0, 2, -2}); 

    float result = computePointLightSpecular(&testLight, testPoint, testNormal, viewDir, 32.0f, &testScene);
    
    // Expected intensity based on inverse square law
    float expectedIntensity = 0.125f;

    TEST_ASSERT_FLOAT_WITHIN(0.001f, expectedIntensity, result);
}

// Test case: Partial specular reflection due to angle
void test_ComputePointLightSpecular_PartialReflection(void)
{
    Scene testScene;
    PointLight testLight;
    Vector testPoint;
    Vector testNormal = (Vector){0, 1, 0}; // Upward-facing surface

    // Initialize scene
    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial pointLightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f}; // Full intensity
    Vector pointLightPosition = (Vector){0, 2, 2}; // Slightly angled light

    testLight = (PointLight){pointLightPosition, 5, pointLightMaterial};

    testPoint = (Vector){0, 0, 0};

    Vector viewDir = normalizeVector((Vector){0, 0.65f, -1.0f});
 
    float result = computePointLightSpecular(&testLight, testPoint, testNormal, viewDir, 32.0f, &testScene);
    
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, result);  
}

// Test case: Normal facing away from light, no specular reflection expected
void test_ComputeDirectionalLightSpecular_NormalFacingAway(void);

// Test case: Perfect specular reflection with aligned vectors
void test_ComputeDirectionalLightSpecular_PerfectReflection(void);

// Test case: Partial specular reflection due to angled light source
void test_ComputeDirectionalLightSpecular_PartialReflection(void);

// Test case: High shininess results in a small sharp highlight
void test_ComputeDirectionalLightSpecular_HighShininess(void);

// Test case: Low shininess results in a broad weak highlight
void test_ComputeDirectionalLightSpecular_LowShininess(void);

// Test case: Point in shadow, no specular contribution
void test_ComputeDirectionalLightSpecular_PointInShadow(void);

// Test case: Point outside the spotlight cone, no specular reflection
void test_ComputeSpotLightSpecular_PointOutsideCone(void);

// Test case: Point inside the spotlight cone, perfect reflection
void test_ComputeSpotLightSpecular_PerfectReflection(void);

// Test case: Point at the edge of the spotlight cone, weaker reflection
void test_ComputeSpotLightSpecular_EdgeOfCone(void);

// Test case: High shininess results in a small sharp highlight
void test_ComputeSpotLightSpecular_HighShininess(void);

// Test case: Low shininess results in a broad highlight
void test_ComputeSpotLightSpecular_LowShininess(void);

// Test case: Point in shadow, no specular contribution
void test_ComputeSpotLightSpecular_PointInShadow(void);
