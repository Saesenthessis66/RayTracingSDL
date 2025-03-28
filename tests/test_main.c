#include "unity.h"

// Declare test functions
void test_vectorLength(void);
void test_multiplyVector(void);
void test_addVectors(void);
void test_subtractVectors(void);
void test_normalizeVector(void);
void test_vectorCrossProduct(void);
void test_dotProduct(void);

void test_initCamera(void);
void test_mapPixelToRay(void);

void test_computeSphereNormal(void);
void test_computeTriangleNormal(void);
void test_intersectRaySphere(void);
void test_intersectRayPlane(void);
void test_intersectRayTriangle(void);

void test_computePointLightIntensity(void);
void test_computeDirectionalLightIntensity(void);
void test_computeSpotLightIntensity(void);
void test_computeAmbientLightIntensity(void);

void test_initScene(void);
void test_addSphere(void);
void test_addPlane(void);
void test_addTriangle(void);
void test_addPointLight(void);
void test_addDirectionalLight(void);
void test_addSpotLight(void);
void test_setAmbientLight(void);
void test_isPointInShadow(void);
void test_isPointInShadowDir(void);
void test_isPointInShadowSpot(void);

void test_ComputePointLightDiffuse_PointInShadow(void);
void test_ComputePointLightDiffuse_ZeroLightIntensity(void);
void test_ComputePointLightDiffuse_NormalFacingAway(void);
void test_ComputePointLightDiffuse_ValidDiffuse(void);
void test_ComputeDirectionalLightDiffuse_PointInShadow(void);
void test_ComputeDirectionalLightDiffuse_NormalFacingAway(void);
void test_ComputeDirectionalLightDiffuse_ValidDiffuse(void);
void test_ComputeSpotLightDiffuse_PointInShadow(void);
void test_ComputeSpotLightDiffuse_ZeroLightIntensity(void);
void test_ComputeSpotLightDiffuse_NormalFacingAway(void);
void test_ComputeSpotLightDiffuse_ValidDiffuse(void);

void test_ComputePointLightSpecular_PointInShadow(void);
void test_ComputePointLightSpecular_PerfectReflection(void);
void test_ComputePointLightSpecular_PartialReflection(void);
void test_ComputeDirectionalLightSpecular_NormalFacingAway(void);
void test_ComputeDirectionalLightSpecular_PerfectReflection(void);
void test_ComputeDirectionalLightSpecular_PartialReflection(void);
void test_ComputeDirectionalLightSpecular_PointInShadow(void);
void test_ComputeSpotLightSpecular_PointOutsideCone(void);
void test_ComputeSpotLightSpecular_PerfectReflection(void);
void test_ComputeSpotLightSpecular_EdgeOfCone(void);
void test_ComputeSpotLightSpecular_PointInShadow(void);

void test_ComputeSurfaceColor_NoLights(void);
void test_ComputeSurfaceColor_MultipleLights(void);
void test_ComputeSurfaceColor_HighShininess(void);
void test_ComputeSurfaceColor_LowShininess(void);
void test_ComputeSurfaceColor_LightOppositeNormal(void);
void test_ComputeSurfaceColor_MaxIntensityClamping(void);

void setUp(void) {}   // Runs before each test (optional)
void tearDown(void) {} // Runs after each test (optional)

int main(void) {
    UNITY_BEGIN();

    printf("\n===== Running Ray Tests =====\n");
    RUN_TEST(test_vectorLength);
    RUN_TEST(test_multiplyVector);
    RUN_TEST(test_addVectors);
    RUN_TEST(test_subtractVectors);
    RUN_TEST(test_normalizeVector);
    RUN_TEST(test_vectorCrossProduct);
    RUN_TEST(test_dotProduct);

    printf("\n===== Running Camera Tests =====\n");
    RUN_TEST(test_initCamera);
    RUN_TEST(test_mapPixelToRay);

    printf("\n===== Running Shapes Tests =====\n");
    RUN_TEST(test_computeSphereNormal);
    RUN_TEST(test_computeTriangleNormal);
    RUN_TEST(test_intersectRaySphere);
    RUN_TEST(test_intersectRayPlane);
    RUN_TEST(test_intersectRayTriangle);

    printf("\n===== Running Light Source Tests =====\n");
    RUN_TEST(test_computePointLightIntensity);
    RUN_TEST(test_computeDirectionalLightIntensity);
    RUN_TEST(test_computeSpotLightIntensity);
    RUN_TEST(test_computeAmbientLightIntensity);

    printf("\n===== Running Scene Tests =====\n");
    RUN_TEST(test_initScene);
    RUN_TEST(test_addSphere);
    RUN_TEST(test_addPlane);
    RUN_TEST(test_addTriangle);
    RUN_TEST(test_addPointLight);
    RUN_TEST(test_addDirectionalLight);
    RUN_TEST(test_addSpotLight);
    RUN_TEST(test_setAmbientLight);
    RUN_TEST(test_isPointInShadow);
    RUN_TEST(test_isPointInShadowDir);
    RUN_TEST(test_isPointInShadowSpot);

    printf("\n===== Running Illumination Diffuse Tests =====\n");
    RUN_TEST(test_ComputePointLightDiffuse_PointInShadow);
    RUN_TEST(test_ComputePointLightDiffuse_ZeroLightIntensity);
    RUN_TEST(test_ComputePointLightDiffuse_NormalFacingAway);
    RUN_TEST(test_ComputePointLightDiffuse_ValidDiffuse);
    RUN_TEST(test_ComputeDirectionalLightDiffuse_PointInShadow);
    RUN_TEST(test_ComputeDirectionalLightDiffuse_NormalFacingAway);
    RUN_TEST(test_ComputeDirectionalLightDiffuse_ValidDiffuse);
    RUN_TEST(test_ComputeSpotLightDiffuse_PointInShadow);
    RUN_TEST(test_ComputeSpotLightDiffuse_ZeroLightIntensity);
    RUN_TEST(test_ComputeSpotLightDiffuse_NormalFacingAway);
    RUN_TEST(test_ComputeSpotLightDiffuse_ValidDiffuse);

    printf("\n===== Running Illumination Specular Tests =====\n");
    RUN_TEST(test_ComputePointLightSpecular_PointInShadow);
    RUN_TEST(test_ComputePointLightSpecular_PerfectReflection);
    RUN_TEST(test_ComputePointLightSpecular_PartialReflection);
    RUN_TEST(test_ComputeDirectionalLightSpecular_NormalFacingAway);
    RUN_TEST(test_ComputeDirectionalLightSpecular_PerfectReflection);
    RUN_TEST(test_ComputeDirectionalLightSpecular_PartialReflection);
    RUN_TEST(test_ComputeDirectionalLightSpecular_PointInShadow);
    RUN_TEST(test_ComputeSpotLightSpecular_PointOutsideCone);
    RUN_TEST(test_ComputeSpotLightSpecular_PerfectReflection);
    RUN_TEST(test_ComputeSpotLightSpecular_EdgeOfCone);
    RUN_TEST(test_ComputeSpotLightSpecular_PointInShadow);

    printf("\n===== Running Illumination Surface Color Tests =====\n");
    RUN_TEST(test_ComputeSurfaceColor_NoLights);
    RUN_TEST(test_ComputeSurfaceColor_MultipleLights);
    RUN_TEST(test_ComputeSurfaceColor_HighShininess);
    RUN_TEST(test_ComputeSurfaceColor_LowShininess);
    RUN_TEST(test_ComputeSurfaceColor_LightOppositeNormal);
    RUN_TEST(test_ComputeSurfaceColor_MaxIntensityClamping);

    return UNITY_END();
}