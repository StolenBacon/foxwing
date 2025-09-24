#include <raylib.h>

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
constexpr int kTargetFPS = 60;

int main(void) {

    InitWindow(kWindowWidth, kWindowHeight, "Foxwing");

    // Define the camera to look into our 3d world
    Camera3D camera = {0};
    camera.position = Vector3{10.0f, 10.0f, 10.0f}; // Camera position
    camera.target = Vector3{0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = Vector3{0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(kTargetFPS); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('Z'))
            camera.target = Vector3{0.0f, 0.0f, 0.0f};

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, BLUE);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, RED);

        DrawGrid(10, 1.0f);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}