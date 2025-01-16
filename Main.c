#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "shader.h"
#include "utils.h"

#define WIDTH 80
#define HEIGHT 40
#define FPS 12

int main(){
    
    vec3 vertices[] = {
    //front
    -0.5, 0.5, -0.5,
    -0.5, -0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, -0.5, -0.5,
    //back
    -0.5, 0.5, 0.5,
    -0.5, -0.5, 0.5,
    0.5, 0.5, 0.5,
    0.5, -0.5, 0.5,

        

    };
    //left to right in terms of x
    int indices[] = {
        0,1,2,
        1,2,3,
        4,5,6,
        5,6,7,
        0,1,4,
        4,5,1,
        2,3,6,
        6,7,3,
        0,2,4,
        4,2,6,

    

    };

    //setup rotation here
    vec3 model_rotation = {10,0,0};

    //set up initial rotation
    vec3 model_init_rotation = {0, 30, 0};


    //setup translation here
    vec3 model_translation = {0, 0, 0};

    

    //setup camera here
    float far = 70;
    float near = 1;

    vec3 camera_trans = {0, 0, 0};
    //camera rotation
    vec3 camera_rot = {0,0,0};
    //focal lenght. greater values will zoom it in
    float focal = 1;

    //setup fov here
    float fov= 90;


    vec3 view_rotation = {0, 0, 0};
    vec3 view_translation = {0, 0, 0};
    vec3 total_rotation = {0,0,0};
    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    vec3 modTrans[n];
    vec3 modVert[n];
    vec3 viewM[n];
    vec3 projection[n];
    vec3 cam[n];

    //initialize the screen
    char screen[HEIGHT * WIDTH + 10];
    int point_len = HEIGHT * WIDTH * 1.5;
    vec3 *points;
    while(1){


        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }

        //edit if needed
        total_rotation.x += model_rotation.x;
        total_rotation.y += model_rotation.y;
        total_rotation.z += model_rotation.z;
        view_rotation.x += camera_rot.x;
        view_rotation.y += camera_rot.y;
        view_rotation.z += camera_rot.z;
        view_translation.x += camera_trans.x;
        view_translation.y += camera_trans.y;
        view_translation.z += camera_trans.z;

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
        scale(modVert, n, 25);


        model(modVert, n, (vec3) {total_rotation.x + model_init_rotation.x, total_rotation.y + model_init_rotation.y, total_rotation.z + model_init_rotation.z});

        translation(modVert, n, model_translation);

        //setup our view
        for(int i = 0; i < n; i++){
            viewM[i] = modVert[i];
        }
       
       //transform based on view space
        //view(viewM, n, tX, tY, tZ, vX, vY, vZ);
    
        for(int i = 0; i < n; i++){
            projection[i] = viewM[i];
        }
        //transform to projection space
        proj(projection, n, far, near, fov, WIDTH, HEIGHT);

        for(int i = 0; i < n; i++){
            cam[i] = projection[i];
           
        }
        //transform based on focal length
        camera(cam, n, focal);

        //convert 
        for(int i = 0; i < n; i++){
            //pixels[i] = toPixel(&cam[i], WIDTH, HEIGHT);
            terminal[i] = toTerminal(&cam[i], WIDTH, HEIGHT);
        }

        printf("%f %f %f\n", terminal[0].z, terminal[1].z, terminal[2].z);
        //number of points per line total
        //int point_len = pointsLen(terminal, indices, 3, sizeof(indices)/sizeof(int));
        //temporary scale our point

        //see if our modified lines are working properly
        points = malloc(point_len * sizeof(vec3));
        int offset = 0;
        int stride = 3;
        int shapes = (sizeof(indices)/sizeof(int))/stride;
        for(int i = 0; i < shapes; i++){
            makeShape(terminal, indices, points, stride, i, &offset);
        }
        printf("%f %f %f\n", points[0].z, points[1].z, points[2].z);
        


        int final_points_len = zBuffer(points, offset + 1, WIDTH, HEIGHT);
        vec3 final_points[final_points_len];
        //transfer our data
        for(int i = 0; i < final_points_len; i++){
            final_points[i] = points[i];
            final_points[i].z =final_points[i].z *  1/(far - near) - (near)/(far - near); 
        }

        printf("%f %f %f\n", final_points[0].z, final_points[1].z, final_points[2].z);
        
        //attempt to render
        render(final_points, final_points_len, WIDTH, HEIGHT, screen);
        wait(FPS);
        clear();
        //free(screen);


    }
    



    return 0;
    
}