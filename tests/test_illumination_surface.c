#include "unity.h"
#include "scene.h"
#include "illumination.h"

bool areColorsEqual(SDL_Color c1, SDL_Color c2, int tolerance)
{
    return abs(c1.r - c2.r) <= tolerance &&
           abs(c1.g - c2.g) <= tolerance &&
           abs(c1.b - c2.b) <= tolerance &&
           c1.a == c2.a;
}

// Tests surface color computation when there are no active lights in the scene.
void test_ComputeSurfaceColor_NoLights(void)
{

}

// Tests surface color computation when multiple light sources contribute to the final color.
void test_ComputeSurfaceColor_MultipleLights(void)
{

}

// Tests surface color computation for a highly reflective material with maximum shininess.
void test_ComputeSurfaceColor_HighShininess(void)
{

}

// Tests surface color computation for a material with low shininess, ensuring a soft specular highlight.
void test_ComputeSurfaceColor_LowShininess(void)
{

}

// Tests specular reflection when the view direction is slightly misaligned from the perfect reflection vector.
void test_ComputeSurfaceColor_SpecularAtAngle(void)
{

}

// Tests surface color computation for a purely specular material with no diffuse component.
void test_ComputeSurfaceColor_PurelySpecular(void)
{

}

// Tests that a surface facing away from the light does not receive direct illumination.
void test_ComputeSurfaceColor_LightOppositeNormal(void)
{

}

// Tests that the final computed color does not exceed the maximum RGB range when multiple strong lights are present.
void test_ComputeSurfaceColor_MaxIntensityClamping(void)
{
    
}
