#include <stdio.h>
#include <stdlib.h>
#include<process.h>
#include <time.h>


#include "include/camera.h"
#include "include/frag.h"
#include "include/renderer.h"
#include "include/shader.h"
#include "include/utils.h"
#include "include/mesh.h"

#define WIDTH 100
#define HEIGHT 50
#define FPS 12
#define TICK 0.1

int main(){
    


    vec3 vertices[] = {
    //front
    -1.0, -1.0, -1.0,
    -1.0, 1.0, -1.0,
    1.0, 1.0, -1.0,
    1.0, -1.0, -1.0,

    //back
    -1.0, -1.0, 1.0,
    -1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, -1.0, 1.0,

        

    };
    //left to right in terms of x
    int indices[] = {
        0, 1, 2,
        0, 2, 3,
        7, 6, 5,
        7, 5, 4,
        3, 2, 6,
        3, 6, 7,
        4, 5, 1,
        4, 1, 0,
        4, 0, 3,
        4, 3, 7,
        1, 5, 6,
        1, 6, 2 
    };

    Mesh box;
    box.vertices = vertices;
    box.indices = indices;
    
    //try bunny
    Mesh bunny;
    if(loadMesh(&bunny, "bunny.obj") != 0){
        printf("Failed to load bunny.obj\n");
        return 1;
    }

    int indCount = bunny.indexCount;


    int indCount1 = sizeof(indices)/sizeof(int);


    //setup rotation h
    vec3 model_rotation = {0, 45, 0};

    //set up initial rotation
    vec3 model_init_rotation = {0, 10, 0};
    box.rotation = model_init_rotation;
    setMeshRotation(&bunny, model_init_rotation);

    //setup translation here
    vec3 model_translation = {0, -0.5, -10};
    box.position = model_translation;
    setMeshPosition(&bunny, model_translation);

    //setup camera here
    Camera cam = {{0, 0, 0}, {0, 0, 0}, 1.0};

    float far_d = 100;
    float near_d = 1.0;


    //setup fov here
    float fov= 70;

    vec3 total_rotation = model_rotation;
    int n = bunny.vertexCount;
    vec3 * terminal = (vec3*) malloc(sizeof(vec3) * n);


    //initialize the screen
    char screen[HEIGHT * WIDTH + HEIGHT];

    Frag* frag = makeFrag(WIDTH, HEIGHT);

    float elapsed = 0;
    float elapsed_tick = 0;
    //vec3 *points;
    while(1){
        
        clock_t start = clock();
        clock_t tick_start = clock();
        
        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            terminal[i] = bunny.vertices[i];
        }

        //edit if needed
        if(elapsed_tick >= TICK){
            total_rotation.x += bunny.rotation.x;
            total_rotation.y += bunny.rotation.y;
            total_rotation.z += bunny.rotation.z;
            elapsed_tick = 0;
        }
        


        modelTransform(terminal, n, 30.0, total_rotation,  bunny.position);

        int stride = 3;
        int shapes = indCount/3;


        // Clear screen before rendering
        printf("\033[2J\033[H\033[3J");

        
        draw(terminal, bunny.indices, shapes, stride, near_d, far_d, fov, cam, frag, screen, WIDTH, HEIGHT);
        
        clock_t end = clock();
        elapsed_tick += (end - tick_start) / (float) CLOCKS_PER_SEC;

        elapsed = end - start;
        int fps = CLOCKS_PER_SEC / elapsed;
        elapsed /= CLOCKS_PER_SEC;
        elapsed *= 1000;
        printf("FPS: %d | Frametime: %.2f ms\n", fps, elapsed); 
        //free(points);
        //points = NULL;
        //free(screen);
    }
    free(frag);
    

    return 0;
    
}