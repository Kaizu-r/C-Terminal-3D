#include <stdio.h>
#include <stdlib.h>

#include "frag.h"
#include "renderer.h"
#include "shader.h"
#include "utils.h"

#define WIDTH 100
#define HEIGHT 50
#define FPS 24

int main(){
    
    vec3 vertices[] = {
    //front
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,

    //back
    0.0, 0.0, 1.0,
    0.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 0.0, 1.0,

        

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



    int indCount1 = sizeof(indices)/sizeof(int);


    //setup rotation here
    vec3 model_rotation = {0, 5, 0};

    //set up initial rotation
    vec3 model_init_rotation = {45, 45, 45};


    //setup translation here
    vec3 model_translation = {0, 0, 3.0};

    //light
    vec3 light = {0, 10, -10};
    //light = toTerminal(&light, WIDTH, HEIGHT);

    //setup camera here
    vec3 cam = {0, 0, 0};
    float far = 1000;
    float near = 0.1;

    
    //focal lenght. greater values will zoom it in
    float focal = 1;

    //setup fov here
    float fov= 90;


    vec3 view_rotation = {0, 0, 0};
    vec3 view_translation = {0, 0, 0};
    vec3 total_rotation = model_init_rotation;
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 terminal[n];
    vec3 modVert[n];

    vec3 projection[n];


    //initialize the screen
    char screen[HEIGHT * WIDTH + HEIGHT];
    int point_len = HEIGHT * WIDTH * 1.5;
    Frag* frag = makeFrag(WIDTH, HEIGHT);
    //vec3 *points;
    while(1){
        
        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }

        //edit if needed
        total_rotation.x += model_rotation.x;
        total_rotation.y += model_rotation.y;
        total_rotation.z += model_rotation.z;


        modelTransform(modVert, n, 0.75, total_rotation,  model_translation);


        for(int i = 0; i < n; i++){
            terminal[i] = modVert[i];
        }



        
        int offset = 0;
        int stride = 3;
        int shapes = (sizeof(indices)/sizeof(int))/stride;
        for(int i = 0; i < shapes; i++){
            tri tri1 = triangleBuild(terminal, indices, i);

            vec3 norm = normal(tri1.v1, tri1.v2, tri1.v3);

            vec3 distance_to_cam = {tri1.v1.x - cam.x, tri1.v1.y - cam.y, tri1.v1.z - cam.z};
            if(dot(norm, distance_to_cam) < 0){
                tri tri2 = tri1;
                proj(&tri2.v1, far, near, fov, WIDTH, HEIGHT);
                proj(&tri2.v2, far, near, fov, WIDTH, HEIGHT);
                proj(&tri2.v3, far, near, fov, WIDTH, HEIGHT);

                tri2.v1 = toTerminal(tri2.v1, WIDTH, HEIGHT);
                tri2.v2 = toTerminal(tri2.v2, WIDTH, HEIGHT);
                tri2.v3 =  toTerminal(tri2.v3, WIDTH, HEIGHT);
                printf("%f, %f, %f\n", tri2.v1.x, tri2.v1.y, tri2.v1.z);

                float light_value;

                //normalize light
                float light_len = fast_inRoot(light.x*light.x + light.y*light.y + light.z*light.z);
                light.x *= light_len;
                light.y *= light_len;
                light.z *= light_len;

                light_value = dot(norm, light) + 1;

                List * list = createList();
                
                drawTriangle(tri2, &list, WIDTH, HEIGHT, light_value);
                
                
                fillTriangle(&list, WIDTH, HEIGHT);
                placeFrag(frag, list, WIDTH, HEIGHT);
        
                
                
            }
        }
        render(frag, WIDTH, HEIGHT, screen);
        resetFrag(frag, WIDTH, HEIGHT);

        
        
        
        wait(FPS);
        //break;
        system("cls");
        //free(points);
        //points = NULL;
        //free(screen);
    }
    free(frag);
    

    return 0;
    
}