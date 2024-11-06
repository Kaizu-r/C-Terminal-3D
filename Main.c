#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "lineShader.h"
#include "utils.h"

#define WIDTH 70
#define HEIGHT 30
#define FPS 12

int main(){
    vec3 vertices[] = {
        -0.35, 0.35, -0.35,    //front upper left
        -0.35, -0.35, -0.35,   //front lower left
        0.35, 0.35, -0.35,     //front upper right
        0.35, -0.35, -0.35,    //front lower right
        -0.35, 0.35, 0.35,    //back upper left
        -0.35, -0.35, 0.35,   //back lower left
        0.35, 0.35, 0.35,     //back upper right
        0.35, -0.35, 0.35,    //back lower right
        

 
    };
    //left to right in terms of x
    int indices[] = {
    
    //front
        0, 1, 2,
        1, 2, 3,
    //back
        4, 5, 6,
        5, 6, 7,
    //left
        0, 1, 4,
        1, 4, 5,
    //right
        2, 3, 6,
        3, 6, 7,
    //top
        0, 2, 4,
        2, 4, 6,
    //bottom
        1, 3, 5,
        3, 5, 7

    

    };

    //setup rotation here
    int xRot = 0;
    int yRot = 10;
    int zRot = 0;

    //set up initial rotation
    int inX = 0;
    int inY = 0;
    int inZ = 0;


    //setup translation here
    float xTrans = 0;
    float yTrans = 0;
    float zTrans = 0;

    

    //setup camera here
    float camX = 0;
    float camY = 0.5;
    float camZ = 0.5;
    int camrX = 10;
    int camrY = 0;
    int camrZ = 0;
    float focal = 1;

    //setup fov here
    float fov= 60;


    int vX = 0;
    int vY = 0;
    int vZ = 0;
    float tX = 0;
    float tY = 0;
    float tZ = 0;
    int xR = 0;
    int yR = 0;
    int zR = 0;
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    vec3 modTrans[n];
    vec3 modVert[n];
    vec3 viewM[n];
    vec3 projection[n];
    vec3 cam[n];
    while(1){
        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }

        //edit if needed
        xR += xRot;
        yR += yRot;
        zR += zRot;
        vX += camrX;
        vY += camrY;
        vZ += camrZ;
        tX = camX;
        tY = camY;
        tZ = camZ;

        //for printing
        if(xR > 360){
            xR = xR - 360;
        }
        if(xR < -360)
            xR = xR + 360;
        if(yR > 360){
            yR = yR - 360;
        }
        if(yR < -360)
            yR = yR + 360;
        if(zR > 360){
            zR = zR - 360;
        }
        if(zR < -360)
            zR = zR + 360;
        printf("%d\n", xR);
        printf("%d\n", yR);
        printf("%d\n", zR);
        //translate hte vertex
        //transform
        model(modVert, n, xR, yR, zR);


        translation(modVert, n, xTrans, yTrans, zTrans);
        //translation(modVert, n, xTrans, yTrans, zTrans);
        //setup our view
        for(int i = 0; i < n; i++){
            viewM[i] = modVert[i];
        }
       
       //transform based on view space
        view(viewM, n, tX, tY, tZ, vX, vY, vZ);
    
        

        for(int i = 0; i < n; i++){
            projection[i] = viewM[i];
        }
        proj(projection, n, 1, 0, fov);

        for(int i = 0; i < n; i++){
            cam[i] = projection[i];
            printf("\t%.3f\t%.3f\t%.3f\n", cam[i].x, cam[i].y, cam[i].z);
        }
        camera(cam, n, focal);

        //convert 
        for(int i = 0; i < n; i++){
            pixels[i] = toPixel(&cam[i], WIDTH, HEIGHT);
            terminal[i] = toTerminal(&pixels[i], WIDTH, HEIGHT);
        }

        int point_len = pointsLen(terminal, indices, 3, sizeof(indices)/sizeof(int));

        //temporary scale our points
        for(int i = 0; i < n; i++){
            terminal[i].z = terminal[i].z * 20 + 20;
        }

        //see if our modified lines are working properly
        vec3 points[point_len];
        
        int offset = 0;
        int stride = 3;
        int shapes = (sizeof(indices)/sizeof(int))/stride;
        for(int i = 0; i < shapes; i++){
            makeShape(terminal, indices, points, stride, i, &offset);
        }

        //normalize our zs
        for(int i = 0; i < point_len; i++)
            points[i].z = (points[i].z - 20)/20;

        int final_points_len = zBuffer(points, point_len, WIDTH, HEIGHT);
        vec3 final_points[final_points_len];
        //transfer our data
        for(int i = 0; i < final_points_len; i++){
            final_points[i] = points[i];
        }

        //test our new points
        //attempt to render
        render(final_points, final_points_len, WIDTH, HEIGHT, '-');
        wait(FPS);
        clear();
    }
    



    return 0;
    
}