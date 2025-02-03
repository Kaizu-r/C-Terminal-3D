#include <stdio.h>
#include <stdlib.h>

#include "frag.h"
#include "renderer.h"
#include "shader.h"
#include "utils.h"

#define WIDTH 120
#define HEIGHT 60
#define FPS 60

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
    vec3 model_rotation = {10,0,0};

    //set up initial rotation
    vec3 model_init_rotation = {0, 30, 0};


    //setup translation here
    vec3 model_translation = {0, 0, 3.0};

    //light
    vec3 light = {0, 1, 0};
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

    float **frag = makeFrag(WIDTH, HEIGHT);

    //initialize the screen
    char screen[HEIGHT * WIDTH + HEIGHT];
    int point_len = HEIGHT * WIDTH * 1.5;
    //vec3 *points;
    while(1){
        resetFrag(frag, WIDTH, HEIGHT);
        
        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }

        //edit if needed
        total_rotation.x += model_rotation.x;
        total_rotation.y += model_rotation.y;
        total_rotation.z += model_rotation.z;

        //for printing
        if(total_rotation.x > 360){
            total_rotation.x -= 360;
        }
        if(total_rotation.x < -360)
            total_rotation.x += 360;
        if(total_rotation.y > 360){
            total_rotation.y -= 360;
        }
        if(total_rotation.y < -360)
            total_rotation.y += 360;
        if(total_rotation.z > 360){
            total_rotation.z -= 360;
        }
        if(total_rotation.z < -360)
            total_rotation.z += 360;
        //printf("%d\n", xR);
        //printf("%d\n", yR);
        //printf("%d\n", zR);
                
        //rotate initially, then rotate based on the incrementing rotation

        modelTransform(modVert, n, 0.75, total_rotation,  model_translation);


        for(int i = 0; i < n; i++){
            terminal[i] = modVert[i];
        }



        printf("%f %f %f\n", terminal[0].z, terminal[1].z, terminal[2].z);
        
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
                tri2.v3 = toTerminal(tri2.v3, WIDTH, HEIGHT);

                float light_value;

                //normalize light
                float light_len = fast_inRoot(light.x*light.x + light.y*light.y + light.z*light.z);
                light.x *= light_len;
                light.y *= light_len;
                light.z *= light_len;

                light_value = dot(norm, light) * -1;

                fillTriangle(tri2, frag, WIDTH, HEIGHT, light_value);
                drawTriangle(tri2, frag, WIDTH, HEIGHT, light_value);
            }
        }
        
        render(frag, WIDTH, HEIGHT, screen);
        wait(FPS);
        //break;
        system("cls");
        //free(points);
        //points = NULL;
        //free(screen);
    }
    

    return 0;
    
}