#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "shader.h"
#include "utils.h"

#define WIDTH 100
#define HEIGHT 50
#define FPS 12

int main(){
    
    vec3 vertices[] = {
    //front
    -0.5, 0.5, -0.5, 0,
    -0.5, -0.5, -0.5, 0,
    0.5, 0.5, -0.5, 0,
    0.5, -0.5, -0.5, 0,
    //back
    -0.5, 0.5, 0.5, 0,
    -0.5, -0.5, 0.5, 0,
    0.5, 0.5, 0.5, 0,
    0.5, -0.5, 0.5, 0,

        

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
       // 0,2,4,
        //4,2,6,
    };

    vec3 vertices2[] = {
    //front
    -0.5, 0.5, -0.5, 0,
    -0.5, -0.5, -0.5, 0,
    0.5, 0.5, -0.5, 0,
    0.5, -0.5, -0.5, 0,

    -0.5, 0.5, 0.5, 0,
    -0.5, -0.5, 0.5, 0,
    0.5, 0.5, 0.5, 0,
    0.5, -0.5, 0.5, 0,
        

    };
    vec3 ground[] ={
        -0.5, 0.5, -0.5, 0,
        -0.5, -0.5, -0.5, 0,
        0.5, 0.5, -0.5, 0,
        0.5, -0.5, -0.5, 0,

        -0.5, 0.5, 0.5, 0,
        -0.5, -0.5, 0.5, 0,
        0.5, 0.5, 0.5, 0,
        0.5, -0.5, 0.5, 0,
    };
    int ground_ind[] ={
        0,1,2,
        1,2,3,
        4,5,6,
        5,6,7,
        0,1,4,
        4,5,1,
        2,3,6,
        6,7,3,
        //0,2,4,
        //4,2,6,
    };
    //left to right in terms of x
    int indices2[] = {
        0,1,2,
        1,2,3,
        4,5,6,
        5,6,7,
        0,1,4,
        4,5,1,
        2,3,6,
        6,7,3,
        //0,2,4,
        //4,2,6,
    };

    int indCount1 = sizeof(indices)/sizeof(int);
    int indCount2 = sizeof(indices2)/sizeof(int);
    int indCount3 = sizeof(ground_ind)/sizeof(int);
    int ind_res = indCount1 + indCount2 + indCount3;

    int comb_ind[indCount1 + indCount2];
    int total_ind[ind_res];
    //setup rotation here
    vec3 model_rotation = {10,10,0};

    //set up initial rotation
    vec3 model_init_rotation = {0, 0, 0};


    //setup translation here
    vec3 model_translation = {20, 0, -10};

    //light
    vec3 light = {0, -10, 0};

    //setup camera here
    float far = 70;
    float near = 0.2;

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
    int m = sizeof(vertices2)/sizeof(vec3);
    int g = sizeof(ground)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n + m + g];
    vec3 modTrans[n];
    vec3 modVert[n];
    vec3 modVert2[m];
    vec3 groundVert[g];
    vec3 comb[n + m];
    vec3 viewM[n + m + g];
    vec3 projection[n + m + g];
    vec3 cam[n + m + g];

    //initialize the screen
    char screen[HEIGHT * WIDTH + 10];
    int point_len = HEIGHT * WIDTH * 1.5;
    vec3 *points;
    while(1){


        //copy vertex data to modvert
        for(int i = 0; i < n; i++){
            modVert[i] = vertices[i];
        }
        for(int i = 0; i < m; i++){
            modVert2[i] = vertices2[i];
        }
        for(int i = 0; i < g; i++){
            groundVert[i] = ground[i];
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

        modelTransform(modVert, n, 20, (vec3) {total_rotation.x + model_init_rotation.x, total_rotation.y + model_init_rotation.y, total_rotation.z + model_init_rotation.z},  model_translation);
        modelTransform(modVert2, m, 20, (vec3) {total_rotation.x + model_init_rotation.x, -(total_rotation.y + model_init_rotation.y), total_rotation.z + model_init_rotation.z},  (vec3) {-10, -5, 10});
        modelTransform(groundVert, g,20, (vec3) {0, total_rotation.y + model_init_rotation.y, total_rotation.z + model_init_rotation.z},  (vec3) {0, 25, -10});


        merge_models(modVert, modVert2, comb, indices, indices2, comb_ind, n, m, indCount1, indCount2);
        merge_models( comb, groundVert, viewM, comb_ind, ground_ind, total_ind, n + m, g, indCount1 + indCount2, indCount3);

        view(viewM, n + m + g, view_translation, view_rotation);
        for(int i = 0; i < n + m + g; i++){
            projection[i] = viewM[i];
        }
        //transform to projection space
        proj(projection, n + m + g, far, near, fov, WIDTH, HEIGHT);

        for(int i = 0; i < n + m + g; i++){
            cam[i] = projection[i];
           
        }
        //transform based on focal length
        camera(cam, n + m + g, focal);

        //convert 
        for(int i = 0; i < n + m + g; i++){
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
        int shapes = (sizeof(total_ind)/sizeof(int))/stride;
        for(int i = 0; i < shapes; i++){
            //careful not to uncomment
            //emit_light(&terminal[indices[offset]], &terminal[indices[offset + 1]], &terminal[indices[offset + 2]], light, WIDTH, HEIGHT);
            makeShape(terminal, total_ind, points, stride, i, &offset);
        }
        int final_points_len = zBuffer(points, offset + 1, WIDTH, HEIGHT);
        vec3 final_points[final_points_len];
        //transfer our data
        for(int i = 0; i < final_points_len; i++){
            final_points[i] = points[i];
            //printf("%f %f %f\n", final_points[i].x, final_points[i].y, final_points[i].z);
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