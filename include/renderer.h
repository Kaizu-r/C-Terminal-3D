#ifndef RENDERER_H
#define RENDERER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camera.h"
#include "coords.h"
#include "utils.h"
#include "vertex.h"
#include "shader.h"
#include "list.h"


float lightValue(vec3 col);
char toAscii(float z);
void draw(vec3 vertices[], int indices[], int shapes, int stride, float near, float far, float fov, Camera cam, Frag * frag, char * screen, int WIDTH, int HEIGHT);
void render(Frag *frag, int WIDTH, int HEIGHT, char screen[]);



float lightValue(vec3 col){
    //normalize col
    vec3 newCol = normalize(col);
    return (newCol.x + newCol.y + newCol.z)/3.0;
}

char toAscii(float z){

    char* ascii = "$@B%%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'.";
    float col = z;
    int index = col * (strlen(ascii) - 1);
    if(index > strlen(ascii) - 1){
        index = strlen(ascii) - 1;
    }
    return ascii[index];

}

void draw(vec3 vertices[], int indices[], int shapes, int stride, float near, float far, float fov, Camera cam, Frag * frag, char * screen, int WIDTH, int HEIGHT){
        for(int i = 0; i < shapes; i++){
            tri tri1 = triangleBuild(vertices, indices, i);

            vec3 norm = normal(tri1.v1, tri1.v2, tri1.v3);

            vec3 distance_to_cam = {tri1.v1.x - cam.position.x, tri1.v1.y - cam.position.y, tri1.v1.z - cam.position.z};
            if(dot(norm, distance_to_cam) < 0){
                tri tri2 = tri1;
                //move to camera space
                view(&tri2.v1, 1, cam.position, cam.rotation);
                view(&tri2.v2, 1, cam.position, cam.rotation);
                view(&tri2.v3, 1, cam.position, cam.rotation);

                //project the triangle vertices
                proj(&tri2.v1, far, near, fov, WIDTH, HEIGHT);
                proj(&tri2.v2, far, near, fov, WIDTH, HEIGHT);
                proj(&tri2.v3, far, near, fov, WIDTH, HEIGHT);
                
                tri2.v1 = toTerminal(tri2.v1, WIDTH, HEIGHT);
                tri2.v2 = toTerminal(tri2.v2, WIDTH, HEIGHT);
                tri2.v3 =  toTerminal(tri2.v3, WIDTH, HEIGHT);

               
                List * list = createList();
                
                drawTriangle(tri2, &list, WIDTH, HEIGHT);
                
                
                fillTriangle(&list, WIDTH, HEIGHT);
                placeFrag(frag, list, WIDTH, HEIGHT);
        
                
                
            }
        }
        render(frag, WIDTH, HEIGHT, screen);
        resetFrag(frag, WIDTH, HEIGHT);

}


//more in line with traditional rendering
void render(Frag *frag, int WIDTH, int HEIGHT, char screen[]){
    int k = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(frag[fragIndex( j, i, WIDTH, HEIGHT)].flag){
                vec3 col = frag[fragIndex(j, i, WIDTH, HEIGHT)].color;
                // Map NDC z from [-1, 1] to [0, 1] for ASCII mapping
                float depth = (col.z + 1.0f) * 0.5f;
                if(depth < 0.0f) depth = 0.0f;
                if(depth > 1.0f) depth = 1.0f;
                // Invert depth so closer objects are lighter
                depth = 1.0f - depth;
                screen[k++] = toAscii(depth);
            }
            else
                screen[k++] = ' ';
            
        }
        screen[k++] = '\n';
    }
    screen[k++] = '\0';
    printf(screen);
}

#endif

