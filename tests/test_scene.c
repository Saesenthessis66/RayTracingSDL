#include "unity.h"
#include "scene.h"

// Constants for testing
#define MAX_SPHERES 5
#define MAX_PLANES 5
#define MAX_TRIANGLES 5
#define MAX_POINT_LIGHTS 5
#define MAX_DIRECTIONAL_LIGHTS 5
#define MAX_SPOT_LIGHTS 5

void test_initScene(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    TEST_ASSERT_NOT_NULL(scene.objects.spheres);
    TEST_ASSERT_NOT_NULL(scene.objects.planes);
    TEST_ASSERT_NOT_NULL(scene.objects.triangles);
    TEST_ASSERT_EQUAL_INT(MAX_SPHERES, scene.objects.maxSpheres);
    TEST_ASSERT_EQUAL_INT(MAX_PLANES, scene.objects.maxPlanes);
    TEST_ASSERT_EQUAL_INT(MAX_TRIANGLES, scene.objects.maxTriangles);

    TEST_ASSERT_NOT_NULL(scene.lights.pointLights);
    TEST_ASSERT_NOT_NULL(scene.lights.directionalLights);
    TEST_ASSERT_NOT_NULL(scene.lights.spotLights);
    TEST_ASSERT_EQUAL_INT(MAX_POINT_LIGHTS, scene.lights.maxPointLights);
    TEST_ASSERT_EQUAL_INT(MAX_DIRECTIONAL_LIGHTS, scene.lights.maxDirectionalLights);
    TEST_ASSERT_EQUAL_INT(MAX_SPOT_LIGHTS, scene.lights.maxSpotLights);

    freeScene(&scene);
}

void test_addSphere(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    Vector position = {0.0f, 0.0f, 0.0f};
    Material material = {{255, 0, 0, 255}, 0.5f}; // Red color, 50% reflectivity

    addSphere(&scene, position, 1.0f, material);
    TEST_ASSERT_EQUAL_INT(1, scene.objects.sphereCount);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, scene.objects.spheres[0].radius);
    TEST_ASSERT_EQUAL(255, scene.objects.spheres[0].material.color.r);

    freeScene(&scene);
}

void test_addPlane(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    Vector position = {0.0f, 0.0f, 0.0f};
    Vector normal = {0.0f, 1.0f, 0.0f}; // Horizontal plane
    Material material = {{0, 255, 0, 255}, 0.8f}; // Green color, 80% reflectivity

    addPlane(&scene, position, normal, 10.0f, 10.0f, material);
    TEST_ASSERT_EQUAL_INT(1, scene.objects.planeCount);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, scene.objects.planes[0].width);
    TEST_ASSERT_EQUAL(255, scene.objects.planes[0].material.color.g);

    freeScene(&scene);
}

void test_addTriangle(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    Vector v1 = {0.0f, 0.0f, 0.0f};
    Vector v2 = {1.0f, 0.0f, 0.0f};
    Vector v3 = {0.0f, 1.0f, 0.0f};
    Material material = {{0, 0, 255, 255}, 0.3f}; // Blue color, 30% reflectivity

    addTriangle(&scene, v1, v2, v3, material);
    TEST_ASSERT_EQUAL_INT(1, scene.objects.triangleCount);
    TEST_ASSERT_EQUAL(255, scene.objects.triangles[0].material.color.b);

    freeScene(&scene);
}

void test_addPointLight(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f}; // White light, full intensity
    Vector position = {0.0f, 5.0f, 0.0f};

    addPointLight(&scene, material, position, 10.0f);
    TEST_ASSERT_EQUAL_INT(1, scene.lights.pointLightCount);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, scene.lights.pointLights[0].range);

    freeScene(&scene);
}

void test_addDirectionalLight(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f};
    Vector position = {0.0f, 5.0f, 0.0f};
    Vector direction = {0.0f, -1.0f, 0.0f};

    addDirectionalLight(&scene, material, position, direction);
    TEST_ASSERT_EQUAL_INT(1, scene.lights.directionalLightCount);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -1.0f, scene.lights.directionalLights[0].direction.y);

    freeScene(&scene);
}

void test_addSpotLight(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f};
    Vector position = {0.0f, 5.0f, 0.0f};
    Vector direction = {0.0f, -1.0f, 0.0f};

    addSpotLight(&scene, material, position, direction, 30.0f, 15.0f);
    TEST_ASSERT_EQUAL_INT(1, scene.lights.spotLightCount);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, scene.lights.spotLights[0].cutoffAngle);

    freeScene(&scene);
}

void test_setAmbientLight(void) {
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 0.2f};
    setAmbientLight(&scene, material);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, scene.lights.ambientLight.material.intensity);

    freeScene(&scene);
}

void test_isPointInShadow(void){
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f};
    Vector position = {0.0f, 5.0f, 0.0f};

    addPointLight(&scene, material, position, 10.0f);

    Vector positionP = {0.0f, 1.0f, 0.0f};
    Vector normalP = {0.0f, 1.0f, 0.0f}; // Horizontal plane
    Material materialP = {{0, 255, 0, 255}, 0.8f}; // Green color, 80% reflectivity

    addPlane(&scene, positionP, normalP, 10.0f, 10.0f, materialP);

    Vector point = {0,0,0};

    TEST_ASSERT_EQUAL_INT(1, isPointInShadow(point,&scene.lights.pointLights[0],&scene));
}

void test_isPointInShadowDir(void){
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f};
    Vector position = {0.0f, 5.0f, 0.0f};
    Vector direction = {0.0f, 1.0f, 0.0f};

    addDirectionalLight(&scene, material, position, direction);

    Vector positionP = {0.0f, 1.0f, 0.0f};
    Vector normalP = {0.0f, 1.0f, 0.0f}; // Horizontal plane
    Material materialP = {{0, 255, 0, 255}, 0.8f}; // Green color, 80% reflectivity

    addPlane(&scene, positionP, normalP, 10.0f, 10.0f, materialP);

    Vector point = {0,0,0};

    TEST_ASSERT_EQUAL_INT(1, isPointInShadowDir(point,&scene.lights.directionalLights[0],&scene));
}

void test_isPointInShadowSpot(void){
    Scene scene;
    initScene(&scene, MAX_SPHERES, MAX_PLANES, MAX_TRIANGLES, MAX_POINT_LIGHTS, MAX_DIRECTIONAL_LIGHTS, MAX_SPOT_LIGHTS);

    LightMaterial material = {{255, 255, 255, 255}, 1.0f};
    Vector position = {0.0f, 5.0f, 0.0f};
    Vector direction = {0.0f, -1.0f, 0.0f};

    addSpotLight(&scene, material, position, direction, 30.0f, 15.0f);

    Vector positionP = {0.0f, 1.0f, 0.0f};
    Vector normalP = {0.0f, 1.0f, 0.0f}; // Horizontal plane
    Material materialP = {{0, 255, 0, 255}, 0.8f}; // Green color, 80% reflectivity

    addPlane(&scene, positionP, normalP, 10.0f, 10.0f, materialP);

    Vector point = {0,0,0};

    TEST_ASSERT_EQUAL_INT(1, isPointInShadowSpot(point,&scene.lights.spotLights[0],&scene));
}