#ifndef RENDERER_H
#define RENDERER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "camera.h"
#include "shader.h"
#include "list.h"
    

float lightValue(vec3 col);
char toAscii(float z);
void draw(vec3 vertices[], int indices[], int shapes, int stride, Camera cam, Frag * frag, char * screen, int WIDTH, int HEIGHT);
void render(Frag *frag, int WIDTH, int HEIGHT, char screen[]);



float lightValue(vec3 col){
    //normalize col
    vec3 newCol = normalize(col);
    return (newCol.x + newCol.y + newCol.z)/3.0;
}

char toAscii(float z){

    //ascii gradient
    char* ascii = "$@B%%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'.";
    float col = z;

    //maps 0.0 -> 1.0 to 0 -> len(ascii) -1
    int index = col * (strlen(ascii) - 1);

    //clamping
    if(index > strlen(ascii) - 1){
        index = strlen(ascii) - 1;
    }
    return ascii[index];

}

//draw call
void draw(vec3 vertices[], int indices[], int shapes, int stride, Camera cam, Frag * frag, char * screen, int WIDTH, int HEIGHT){
        for(int i = 0; i < shapes; i++){
            //triangle setup
            tri tri1 = triangleBuild(vertices, indices, i);

            //calculate normal
            vec3 norm = normal(tri1.v1, tri1.v2, tri1.v3);

            //backface culling
            vec3 distance_to_cam = {tri1.v1.x - cam.position.x, tri1.v1.y - cam.position.y, tri1.v1.z - cam.position.z};
            
            if(dot(norm, distance_to_cam) < 0){
                tri tri2 = tri1;
                //move to camera space
                view(&tri2.v1, 1, cam.position, cam.rotation);
                view(&tri2.v2, 1, cam.position, cam.rotation);
                view(&tri2.v3, 1, cam.position, cam.rotation);

                // Near plane clipping: skip triangles too close to camera
                // In view space, camera looks down -Z, so valid depth is negative
                if(tri2.v1.z >= -cam.near_plane || tri2.v2.z >= -cam.near_plane || tri2.v3.z >= -cam.near_plane){
                    continue;  // Skip this triangle, it's too close or behind camera
                }

                //project the triangle vertices
                proj(&tri2.v1, cam.far_plane, cam.near_plane, cam.fov_deg, WIDTH, HEIGHT);
                proj(&tri2.v2, cam.far_plane, cam.near_plane, cam.fov_deg, WIDTH, HEIGHT);
                proj(&tri2.v3, cam.far_plane, cam.near_plane, cam.fov_deg, WIDTH, HEIGHT);
                //convert to terminal coords
                tri2.v1 = toTerminal(tri2.v1, WIDTH, HEIGHT);
                tri2.v2 = toTerminal(tri2.v2, WIDTH, HEIGHT);
                tri2.v3 =  toTerminal(tri2.v3, WIDTH, HEIGHT);

               //create list for rasterization
                List * list = createList();
                
                //draw triangle edges
                drawTriangle(tri2, &list, WIDTH, HEIGHT, norm);
                
                //fill triangle
                fillTriangle(&list, WIDTH, HEIGHT);

                //place fragments
                placeFrag(frag, list, WIDTH, HEIGHT);
        
                
                
            }
        }
        //render the frame (duh)
        render(frag, WIDTH, HEIGHT, screen);

        //reset frag for next frame
        resetFrag(frag, WIDTH, HEIGHT);

}


//more in line with traditional rendering
void render(Frag *frag, int WIDTH, int HEIGHT, char screen[]){

    //winows console specific rendering
    static HANDLE hConsole = NULL;
    static CHAR_INFO* buffer = NULL;
    static SMALL_RECT writeRegion;
    
    // Initialize console on first call
    if (!hConsole) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        buffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * WIDTH * HEIGHT);
        writeRegion.Left = 0;
        writeRegion.Top = 0;
        writeRegion.Right = WIDTH - 1;
        writeRegion.Bottom = HEIGHT - 1;
    }
    
    // Build buffer with color info
    int k = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(frag[fragIndex(j, i, WIDTH, HEIGHT)].flag){
                vec3 col = frag[fragIndex(j, i, WIDTH, HEIGHT)].color;
                float depth = (col.z + 1.0f) * 0.5f;
                if(depth < 0.0f) depth = 0.0f;
                if(depth > 1.0f) depth = 1.0f;

                depth = 1.0f - depth;
                buffer[k].Char.AsciiChar = toAscii(depth);
            }
            else {
                buffer[k].Char.AsciiChar = ' ';

            }
            // White text on black background
            buffer[k].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            k++;
        }
    }
    
    // Write directly to console buffer (much faster than printf)
    COORD bufferSize = {WIDTH, HEIGHT};
    COORD bufferCoord = {0, 0};
    WriteConsoleOutputA(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
}

#endif

