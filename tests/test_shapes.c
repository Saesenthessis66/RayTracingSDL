#include "unity.h"
#include "shapes.h"

#define EPSILON 0.0001

void test_computeSphereNormal(void) {
    Sphere sphere = {{0.0f, 0.0f, 0.0f}, 5.0f, {{255, 0, 0, 255}, 0.5f}};
    Vector point = {0.0f, 5.0f, 0.0f};
    Vector normal = computeSphereNormal(point, sphere);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, normal.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, normal.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, normal.z);
}

void test_computeTriangleNormal(void) {
    Triangle triangle = {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {{255, 255, 255, 255}, 0.3f}};
    Vector normal = computeTriangleNormal(triangle, (Vector){0.0f, 0.0f, 1.0f});

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, normal.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, normal.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, normal.z);
}

void test_intersectRaySphere(void) {
    Sphere sphere = {{0.0f, 0.0f, -10.0f}, 2.0f, {{0, 255, 0, 255}, 0.7f}};
    Ray ray = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}};
    float t;

    TEST_ASSERT_EQUAL(1, intersectRaySphere(ray, sphere, &t));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 8.0f, t);
}

void test_intersectRayPlane(void) {
    Plane plane = {{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 4.0f, 4.0f, {{200, 200, 200, 255}, 0.1f}};
    Ray ray = {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}};
    float t;

    TEST_ASSERT_EQUAL(1, intersectRayPlane(ray, plane, &t));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, t);
}

void test_intersectRayTriangle(void) {
    Triangle triangle = {{-1.0f, -1.0f, -5.0f}, {1.0f, -1.0f, -5.0f}, {0.0f, 1.0f, -5.0f}, {{150, 150, 150, 255}, 0.5f}};
    Ray ray = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}};
    float t;

    TEST_ASSERT_EQUAL(1, intersectRayTriangle(ray, triangle, &t));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, t);
}