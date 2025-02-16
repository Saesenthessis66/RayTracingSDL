#include "../src/objects.c"

// Function to test the camera and ray mapping functionality
void testCamera()
{
    // Define camera parameters
    Vector cameraPos = {0.0f, 0.0f, 0.0f};  // Camera at origin
    Vector cameraDir = {0.0f, 0.0f, -1.0f}; // Camera looking along negative Z-axis
    Vector cameraUp = {0.0f, 1.0f, 0.0f};   // Camera's up direction along Y-axis
    float fov = 90.0f;                      // 90-degree field of view
    int screenWidth = 800;
    int screenHeight = 600;

    // Initialize the camera
    Camera camera;
    initCamera(&camera, cameraPos, cameraDir, cameraUp, fov, screenWidth, screenHeight);

    // Print initialized camera parameters
    printf("Camera Initialized:\n");
    printf("Position: (%.2f, %.2f, %.2f)\n", camera.position.x, camera.position.y, camera.position.z);
    printf("Direction: (%.2f, %.2f, %.2f)\n", camera.direction.x, camera.direction.y, camera.direction.z);
    printf("UpVector: (%.2f, %.2f, %.2f)\n", camera.upVector.x, camera.upVector.y, camera.upVector.z);
    printf("RightVector: (%.2f, %.2f, %.2f)\n", camera.rightVector.x, camera.rightVector.y, camera.rightVector.z);
    printf("FOV: %.2f, Aspect Ratio: %.2f\n", camera.fieldOfView, camera.aspectRatio);

    // Test ray generation for specific pixels
    int testPixels[3][2] = { {0, 0}, {screenWidth / 2, screenHeight / 2}, {screenWidth - 1, screenHeight - 1} };

    printf("\nGenerated Rays:\n");
    for (int i = 0; i < 3; i++)
    {
        int x = testPixels[i][0];
        int y = testPixels[i][1];
        Ray ray = mapPixelToRay(&camera, x, y, screenWidth, screenHeight);
        
        printf("Pixel (%d, %d):\n", x, y);
        printf("  Ray Origin: (%.2f, %.2f, %.2f)\n", ray.origin.x, ray.origin.y, ray.origin.z);
        printf("  Ray Direction: (%.2f, %.2f, %.2f)\n", ray.direction.x, ray.direction.y, ray.direction.z);
    }
}

// Main function to run the test
int main()
{
    testCamera();
    return 0;
}
