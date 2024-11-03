#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "lineShader.h"
#include "utils.h"

#define WIDTH 100
#define HEIGHT 50

int main(){
    vec3 vertices[] = {
        -0.25, 0.25, -0.25,    //front upper left
        -0.25, -0.25, -0.25,   //front lower left
        0.25, 0.25, -0.25,     //front upper right
        0.25, -0.25, -0.25,    //front lower right
        -0.25, 0.25, 0.25,    //back upper left
        -0.25, -0.25, 0.25,   //back lower left
        0.25, 0.25, 0.25,     //back upper right
        0.25, -0.25, 0.25,    //back lower right
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

    //setup rotation here
    int xRot = 0;
    int yRot = 10;
    int zRot = 0;

    //setup translation here
    float xTrans = 0.5;
    float yTrans = 0;
    float zTrans = 0;

    //setup camera here
    float camX = -40;
    float camY = 0;
    float camZ = 0;
    float focal = 1;

    //setup fov here
    float fov = 60;

    int xR = 0;
    int yR = 0;
    int zR = 0;
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    while(1){
        //copy vertex data to modvert
        vec3 modVert[n];
        vec3 viewM[n];
        vec3 projection[n];
        vec3 cam[n];
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }

        //edit if needed
        xR += xRot;
        yR += yRot;
        zR += zRot;
        
        //translate hte vertex
        //transform
        model(modVert, n, xR, yR, zR);

        translation(modVert, n, xTrans, yTrans, zTrans);
        //translation(modVert, n, xTrans, yTrans, zTrans);
        //setup our view
        for(int i = 0; i < n; i++){
            viewM[i] = modVert[i];
        }
        //translation(viewM, n, xTrans, yTrans, zTrans);

        //copy set the projection

        

        for(int i = 0; i < n; i++){
            projection[i] = viewM[i];
        }
        proj(projection, n, 1, 0, fov);
        
        for(int i = 0; i < n; i++){
            cam[i] = projection[i];
        }
        camera(cam, n, focal);

        //convert 
        for(int i = 0; i < n; i++){
            pixels[i] = toPixel(&cam[i], WIDTH, HEIGHT);
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
        wait(12);
        clear();
    }
    



    return 0;
    
}