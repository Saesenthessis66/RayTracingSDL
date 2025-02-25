#include "unity.h"
#include "ray.h"
#include <math.h>

// Tolerance for floating-point comparisons
#define EPSILON 0.0001

void test_vectorLength(void) {
    Vector v = {3.0f, 4.0f, 0.0f};
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, vectorLength(v));
}

void test_multiplyVector(void) {
    Vector v = {1.0f, -2.0f, 3.0f};
    Vector result = multiplyVector(v, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -4.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 6.0f, result.z);
}

void test_addVectors(void) {
    Vector u = {1.0f, 2.0f, 3.0f};
    Vector v = {4.0f, -1.0f, 2.0f};
    Vector result = addVectors(u, v);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, result.z);
}

void test_subtractVectors(void) {
    Vector u = {5.0f, 3.0f, 2.0f};
    Vector v = {1.0f, 4.0f, 0.0f};
    Vector result = subtractVectors(u, v);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -1.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, result.z);
}

void test_normalizeVector(void) {
    Vector v = {3.0f, 0.0f, 4.0f};
    Vector result = normalizeVector(v);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.6f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.8f, result.z);
}

void test_vectorCrossProduct(void) {
    Vector u = {1.0f, 0.0f, 0.0f};
    Vector v = {0.0f, 1.0f, 0.0f};
    Vector result = vectorCrossProduct(u, v);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, result.z);
}

void test_dotProduct(void) {
    Vector u = {1.0f, 2.0f, 3.0f};
    Vector v = {4.0f, -5.0f, 6.0f};
    float result = dotProduct(u, v);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 12.0f, result);
}