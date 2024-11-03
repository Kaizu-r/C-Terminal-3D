#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "lineShader.h"
#include "utils.h"

#define WIDTH 50
#define HEIGHT 25

int main(){
    vec3 vertices[] = {
        -0.5, 0.5, -0.5,    //front upper left
        -0.5, -0.5, -0.5,   //front lower left
        0.5, 0.5, -0.5,     //front upper right
        0.5, -0.5, -0.5,    //front lower right
        -0.5, 0.5, 0.5,    //back upper left
        -0.5, -0.5, 0.5,   //back lower left
        0.5, 0.5, 0.5,     //back upper right
        0.5, -0.5, 0.5,    //back lower right
    };
    //left to right in terms of x
    int indices[] = {
    //front
        0, 1, 2, 
        1, 2, 3,
    //back
        4, 5, 6,
        5, 7, 6,
    //left
        5, 4, 0,
        5, 1, 0,
    //right
        7, 6, 2,
        7, 3, 2 
    };

    int xRot = 0;
    int yRot = 0;
    int zRot = 0;
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    while(1){
        //copy vertex data to modvert
        vec3 modVert[n];
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }
        //setup rotation changes here

        //xRot += 20;
        yRot += 10;
       //zRot += 10;

        //transform
        model(modVert, n, xRot, yRot, zRot);

        //copy model data to projection
        vec3 projection[n];
        for(int i = 0; i < n; i++){
            projection[i] = modVert[i];
        }
        proj(projection, n, 1, 0, 90);
        //convert 
        for(int i = 0; i < n; i++){
            pixels[i] = toPixel(&projection[i], WIDTH, HEIGHT);
            terminal[i] = toTerminal(&pixels[i], WIDTH, HEIGHT);
        }

        int point_len = pointsLen(terminal, indices, 3, sizeof(indices)/sizeof(int));


        //see if our modified lines are working properly
        vec3 points[point_len];
        
        int offset = 0;
        int stride = 3;
        int shapes = (sizeof(indices)/sizeof(int))/stride;
        for(int i = 0; i < shapes; i++){
            makeShape(terminal, indices, points, stride, i, &offset);
        }

        int final_points_len = zBuffer(points, point_len);
        vec3 final_points[final_points_len];
        //transfer our data
        for(int i = 0; i < final_points_len; i++){
            final_points[i] = points[i];
        }

        //test our new points
        //attempt to render
        render(final_points, final_points_len);
        wait(30);
        clear();
    }
    



    return 0;
    
}