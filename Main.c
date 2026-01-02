#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#include "include/renderer.h"
#include "include/mesh.h"

#define WIDTH 100
#define HEIGHT 50
#define FPS 24
#define TICK 0.1f
#define ROTATION_SPEED 90.0f  // degrees per second
#define MOVE_SPEED 5.0f       // units per second

//input handling
static void poll_input(Mesh *target, bool *quit_requested, float delta_time) {
    if (GetAsyncKeyState('Q') & 0x8000) {
        *quit_requested = true;
        return;
    }

    // Movement
    float move_delta = MOVE_SPEED * delta_time;
    if (GetAsyncKeyState('W') & 0x8000) target->position.z += move_delta;
    if (GetAsyncKeyState('S') & 0x8000) target->position.z -= move_delta;
    if (GetAsyncKeyState('A') & 0x8000) target->position.x -= move_delta;
    if (GetAsyncKeyState('D') & 0x8000) target->position.x += move_delta;
    if (GetAsyncKeyState('R') & 0x8000) target->position.y -= move_delta;
    if (GetAsyncKeyState('F') & 0x8000) target->position.y += move_delta;

    // Rotation
    float rotation_delta = ROTATION_SPEED * delta_time;
    if (GetAsyncKeyState('I') & 0x8000) target->rotation.x -= rotation_delta;
    if (GetAsyncKeyState('K') & 0x8000) target->rotation.x += rotation_delta;
    if (GetAsyncKeyState('J') & 0x8000) target->rotation.y -= rotation_delta;
    if (GetAsyncKeyState('L') & 0x8000) target->rotation.y += rotation_delta;
    if (GetAsyncKeyState('U') & 0x8000) target->rotation.z -= rotation_delta;
    if (GetAsyncKeyState('O') & 0x8000) target->rotation.z += rotation_delta;
    
    //scale
    float scale_delta = 5.0f * delta_time;
    if (GetAsyncKeyState('G') & 0x8000) target->scale += scale_delta;
    if (GetAsyncKeyState('H') & 0x8000) target->scale -= scale_delta;
    //reset
    if (GetAsyncKeyState('E') & 0x8000) {
        target->position = (vec3){0.0f, 0.0f, -5.0f};
        target->rotation = (vec3){0.0f, 0.0f, 0.0f};
    }
}

int main(void) {
    
    //load bunny mesh
    Mesh bunny;
    bunny.scale = 1.0f;
    if (loadMesh(&bunny, "bunny.obj") != 0) {
        printf("Failed to load bunny.obj\n");
        return 1;
    }



    // set up initial rotation
    vec3 model_init_rotation = {0.0f, 10.0f, 0.0f};
    setMeshRotation(&bunny, model_init_rotation);

    // setup translation here
    vec3 model_translation = {0.0f, -0.5f, -10.0f};
    setMeshPosition(&bunny, model_translation);

    // setup camera here
    Camera cam = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f};

    //far and near planes
    float far_d = 100.0f;
    float near_d = 1.0f;

    float fov = 70.0f;

    int n = bunny.vertexCount;

    //temp holder for transformed vertices
    vec3 *terminal = (vec3 *)malloc(sizeof(vec3) * n);
    if (!terminal) {
        fprintf(stderr, "Failed to allocate terminal buffer\n");
        freeMesh(&bunny);
        return 1;
    }

    //screen buffer
    char screen[HEIGHT * WIDTH + HEIGHT];
    Frag *frag = makeFrag(WIDTH, HEIGHT);

    printf("Controls: WASD move, R/F rise/fall, I/K/J/L/U/O rotate, Q quit\n");
    printf("Press any key to start...\n");
    getchar();
    system("cls");

    bool quit_requested = false;
    clock_t last_frame = clock();

    while (!quit_requested) {
        //fps setup and delta time calculation
        clock_t start = clock();
        float delta_time = (start - last_frame) / (float)CLOCKS_PER_SEC;
        last_frame = start;

        //copy original vertices to terminal buffer
        for (int i = 0; i < n; i++) {
            terminal[i] = bunny.vertices[i];
        }

        // Poll input every frame with delta time
        poll_input(&bunny, &quit_requested, delta_time);
        if (quit_requested) {
            break;
        }
        //apply model transformations
        modelTransform(terminal, n, bunny.scale, bunny.rotation, bunny.position);

        int stride = 3;
        int shapes = bunny.indexCount / 3;

        // render the frame
        draw(terminal, bunny.indices, shapes, stride, near_d, far_d, fov, cam, frag, screen, WIDTH, HEIGHT);

        //more fps bs and delta time bs
        clock_t end = clock();

        clock_t frame_ticks = end - start;
        int fps = (frame_ticks > 0) ? (int)(CLOCKS_PER_SEC / frame_ticks) : 0;
        float frametime_ms = (frame_ticks / (float)CLOCKS_PER_SEC) * 1000.0f;

        // Show stats in window title instead of printing lines (avoids scroll/flicker)
        char title[128];
        snprintf(title, sizeof(title), "C-Terminal-3D | FPS: %d | Frametime: %.2f ms", fps, frametime_ms);
        SetConsoleTitleA(title);

        // Cap frame rate
        float target_frametime = 1000.0f / FPS;
        if (frametime_ms < target_frametime) {
            Sleep((DWORD)(target_frametime - frametime_ms));
        }
    }

    free(frag);
    free(terminal);
    freeMesh(&bunny);

    return 0;
}