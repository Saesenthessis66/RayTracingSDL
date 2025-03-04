#include "unity.h"
#include "scene.h"
#include "illumination.h"

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
void test_ComputeDirectionalLightSpecular_NormalFacingAway(void)
{
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
    testNormal = normalizeVector((Vector){0, -1, 0});

    Vector viewDir = normalizeVector((Vector){0, 2, -2}); 

    float result = computeDirectionalLightSpecular(&testLight, testPoint, testNormal, viewDir, 32.0f ,&testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Perfect specular reflection with aligned vectors
void test_ComputeDirectionalLightSpecular_PerfectReflection(void)
{
    Scene testScene;
    DirectionalLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){0, 1, -1});

    testLight = (DirectionalLight){(Vector){0, 0, 0}, lightDirection, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = normalizeVector((Vector){0, 1, -1});

    Vector viewDir = normalizeVector((Vector){0, 1, -1}); 

    float result = computeDirectionalLightSpecular(&testLight, testPoint, testNormal, viewDir, 32.0f ,&testScene);
    
    TEST_ASSERT_FLOAT_WITHIN(0.001f, lightMaterial.intensity, result);
}

// Test case: Partial specular reflection due to angled light source
void test_ComputeDirectionalLightSpecular_PartialReflection(void)
{
    Scene testScene;
    DirectionalLight testLight;
    Vector testPoint;
    Vector testNormal;

    // Initialize scene
    initScene(&testScene,5,5,5,5,5,5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 0.5f};
    Vector lightDirection = normalizeVector((Vector){0, 1, -1});

    testLight = (DirectionalLight){(Vector){0, 0, 0}, lightDirection, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = normalizeVector((Vector){0, 1, 0});

    Vector viewDir = normalizeVector((Vector){0, 1, 1}); 

    float result = computeDirectionalLightSpecular(&testLight, testPoint, testNormal, viewDir, 32.0f ,&testScene);
   
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(1.0f, result);  
}

// Test case: Point in shadow, no specular contribution
void test_ComputeDirectionalLightSpecular_PointInShadow(void)
{
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
    Vector viewDir = normalizeVector((Vector){0, 1, 1}); 

    float result = computeDirectionalLightSpecular(&testLight, testPoint, planeNormal, viewDir, 32.0f, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}

// Test case: Point outside the spotlight cone, no specular reflection
void test_ComputeSpotLightSpecular_PointOutsideCone(void)
{
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector lightDirection = normalizeVector((Vector){0, 1, 0});

    testLight = (SpotLight){(Vector){0, 1, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){1, 0, 0};
    testNormal = normalizeVector((Vector){0, 1, 0});

    Vector viewDir = normalizeVector((Vector){0, 1, 0});

    float result = computeSpotLightSpecular(&testLight, testPoint, testNormal,viewDir, 32.0f, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);   
}

// Test case: Point inside the spotlight cone, perfect reflection
void test_ComputeSpotLightSpecular_PerfectReflection(void)
{
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector lightDirection = normalizeVector((Vector){0, 1, 0});

    testLight = (SpotLight){(Vector){0, 1, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0, 0, 0};
    testNormal = normalizeVector((Vector){0, 1, 0});

    Vector viewDir = normalizeVector((Vector){0, 1, 0});

    float result = computeSpotLightSpecular(&testLight, testPoint, testNormal,viewDir, 32.0f, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, result);   
}

// Test case: Point at the edge of the spotlight cone, weaker reflection
void test_ComputeSpotLightSpecular_EdgeOfCone(void)
{
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Vector testNormal;

    initScene(&testScene, 5, 5, 5, 5, 5, 5);

    LightMaterial lightMaterial = (LightMaterial){{255, 255, 255, 255}, 1.0f};
    Vector lightDirection = normalizeVector((Vector){0, 1, 0});

    testLight = (SpotLight){(Vector){0, 1, 0}, lightDirection, 30.0f, 15.0f, lightMaterial};

    testPoint = (Vector){0.3f, 0, 0};
    testNormal = normalizeVector((Vector){0, 1, 0});

    Vector viewDir = normalizeVector((Vector){0, 1, 0});

    float result = computeSpotLightSpecular(&testLight, testPoint, testNormal,viewDir, 32.0f, &testScene);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.1f, result);
    TEST_ASSERT_LESS_THAN_FLOAT(0.9f, result);   
}

// Test case: Point in shadow, no specular contribution
void test_ComputeSpotLightSpecular_PointInShadow(void)
{
    Scene testScene;
    SpotLight testLight;
    Vector testPoint;
    Plane shadowCastingPlane;
    Vector testNormal;

    testNormal = normalizeVector((Vector){0, 1, 0});

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
    Vector viewDir = normalizeVector((Vector){0, 1, 0});

    float result = computeSpotLightSpecular(&testLight, testPoint, testNormal,viewDir, 32.0f, &testScene);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result);
}
