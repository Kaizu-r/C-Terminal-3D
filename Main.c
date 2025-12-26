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
    



    int indCount1 = sizeof(indices)/sizeof(int);


    //setup rotation h
    vec3 model_rotation = {0, 45, 0};

    //set up initial rotation
    vec3 model_init_rotation = {0, 10, 0};
    box.rotation = model_init_rotation;

    //setup translation here
    vec3 model_translation = {0, 0, -3};
    box.position = model_translation;

    //setup camera here
    Camera cam = {{0, 0, 0}, {0, 0, 0}, 1.0};
    float far = 10;
    float near = 1.0;

    //setup fov here
    float fov= 70;

    vec3 total_rotation = model_rotation;
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 terminal[n];
    vec3 modVert[n];

    vec3 projection[n];


    //initialize the screen
    char screen[HEIGHT * WIDTH + HEIGHT];

    Frag* frag = makeFrag(WIDTH, HEIGHT);
    //vec3 *points;
    while(1){
        
        clock_t start = clock();
        
        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = box.vertices[i];
        }

        //edit if needed
        total_rotation.x += box.rotation.x;
        total_rotation.y += box.rotation.y;
        total_rotation.z += box.rotation.z;


        modelTransform(modVert, n, 0.75, total_rotation,  box.position);


        for(int i = 0; i < n; i++){
            terminal[i] = modVert[i];
        }



        int stride = 3;
        int shapes = indCount1/stride;

        draw(terminal, box.indices, shapes, stride, near, far, fov, cam, frag, screen, WIDTH, HEIGHT);
        
        
        
        //wait(FPS);
        //break;
        system("cls");
        clock_t end = clock();
        float elapsed = end - start;
        //elapsed /= CLOCKS_PER_SEC;
        int fps = CLOCKS_PER_SEC / elapsed;
        elapsed /= CLOCKS_PER_SEC;
        elapsed *= 1000;
        printf("FPS: %d\n", fps);
        printf("Frametime: %f ms\n", elapsed); 
        //free(points);
        //points = NULL;
        //free(screen);
    }
    free(frag);
    

    return 0;
    
}