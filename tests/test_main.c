#include "unity.h"

int add(int x, int y)
{
    return x+y;
}

void setUp(void) {
    // Setup code before each test
}

void tearDown(void) {
    // Cleanup code after each test
}

void test_addition(void) {
    TEST_ASSERT_EQUAL(4, add(2, 2));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addition);
    return UNITY_END();
}
