#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"


//draw line in terms of x (bresenham algo)
void lineLow(vec3 vert1, vec3 vert2, float **frag, int n, int WIDTH, int HEIGHT, float l){
    int dx = vert2.x - vert1.x;
    int dy = vert2.y - vert1.y;
    int dz = vert2.z - vert1.z;
    int yi = 1;
    int zi = 1;

    //float z = zGradient(vert1, vert2, 1);

    
    //array for our valid points in the line

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    if(dz < 0){
        zi = -1;
        dz = -dz;
    }
    int D = (dy << 1) - dx;
    int Dz = (dz << 1) - dx;
    int y = vert1.y;
    int z = vert1.z;
    for(int i = 0; i <= n; i++){
        if(y >= 0 && i + (int) vert1.x >= 0
        && y < HEIGHT && i + (int)vert1.x < WIDTH)
            frag[i + (int) vert1.x][y] = l;
        if(D > 0){
            y += yi;
            D += (dy - dx) << 1;
        }
        else
            D += dy << 1;
        if(Dz > 0){
            z += zi;
            Dz += (dz - dx) << 1;
        }
        else{
            Dz += dz << 1;
        }
    }
}

void lineHigh(vec3 vert1, vec3 vert2, float **frag, int n, int WIDTH, int HEIGHT, float l){
    int dx = vert2.x - vert1.x;
    int dy = vert2.y - vert1.y;
    int dz = vert2.z - vert1.z;
    int xi = 1;
    int zi = 1;

    
    /*
    float z;
    //to check for vertical lines
    if(dx == 0)
        z = zGradient(vert1, vert2, 0);
    else
        z = zGradient(vert1, vert2, 1);
    */



    if(dx < 0){
        xi = -1;
        dx = -dx;
    }
    if(dz < 0){
        zi = -1;
        dz = -dz;
    }

    int D = (dx << 1) - dy;
    int Dz = (dz << 1) - dy;
    int x = vert1.x;
    int z = vert1.z;

    //loop in reverse so it is sorted in ascending order
    for(int i = 0; i <= n; i++){
        if(x >= 0 && i + (int) vert1.y >= 0
        && x < WIDTH && i + (int) vert1.y < HEIGHT)
            frag[x][i + (int) vert1.y] = l;
        if(D > 0){
            x += xi;
            D += (dx - dy) << 1;
        }
        else
            D += dx << 1;
        if(Dz > 0){
            z += zi;
            Dz += (dz - dy) << 1;
        }
        else
            Dz += dz << 1;
    }


}

void lineDraw(vec3 vert1, vec3 vert2, float **frag, int WIDTH, int HEIGHT, float l){
    int x0 = vert1.x;
    int y0 = vert1.y;
    int x1 = vert2.x;
    int y1 = vert2.y;
    int n = abs(x0 - x1);
    int m = abs(y0 - y1);
    if(m < n){
        if(x0 > x1)
            lineLow(vert2, vert1, frag, n, WIDTH, HEIGHT, l);
        else
            lineLow(vert1, vert2, frag, n, WIDTH, HEIGHT, l);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, frag, m, WIDTH, HEIGHT, l);
        else
            lineHigh(vert1, vert2, frag, m, WIDTH, HEIGHT, l);
    }
}


//no longer usable
void emit_light(vec3 points[], int point_len, vec3 world[], int world_len, vec3 light, float res, int distance, int WIDTH, int HEIGHT){

    light = toTerminal(light, WIDTH, HEIGHT);
    sortVert(world, world_len);
    int a =0, b = 0, c = 0;
    int screen_points[WIDTH][HEIGHT]; //index of points seen in the screen
    int world_points[WIDTH][HEIGHT][distance]; //index of starting xy pairs
    //set point index to -1
    for(int i = 0; i < WIDTH; i++){
        for(int j= 0; j < HEIGHT; j++){
            screen_points[i][j] = -1;
            for(int k = 0; k < distance; k++){
                world_points[i][j][k] = -1;
            }
            
        }
    }

    for(int i = 0; i < point_len; i++){
            screen_points[(int) points[i].x][(int) points[i].y] = i;    //store index of screen points
    }
    for(int i = 0; i < world_len; i++){
        if(world[i].z + distance/2 >= 0){ //store only within valid distance
            int new_z = world[i].z + distance/2;
            if(world_points[(int) world[i].x][(int) world[i].y][new_z] == -1){  //if still fre
                world_points[(int) world[i].x][(int) world[i].y][new_z] = i;
            }
        }
    }

    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            if((screen_points[i][j] != -1)){//skip step if no point exists
                vec3 dir;
                int index = screen_points[i][j];
                vec3 curr = points[index];
                dir = v_angle(curr, light);
                
                vec3 orig = curr;

                while(1){
                    
                    curr.x += dir.x;
                    curr.y += dir.y;
                    curr.z +=  dir.z;
                    int x_index = (int) curr.x;
                    int y_index = (int) curr.y;
                    int z_index = (int) curr.z;
                    int modified_z = z_index + distance/2.0;

                    
                    if(x_index >= WIDTH || y_index >= HEIGHT ){ //out of bounds, therefore no point
                        break;
                    }
                    
                    if(world_points[x_index][y_index][modified_z] != -1){   //valid point
                        //points[index].l = 0.355;
                        //printf("x");
                        break;
                    }
                    
                    
                }
            }
            



        }
    }




}




void drawTriangle(tri tri1, float **frag, int WIDTH, int HEIGHT, float l){
    
    lineDraw(tri1.v1, tri1.v2, frag, WIDTH, HEIGHT, l);
    lineDraw(tri1.v2, tri1.v3, frag, WIDTH, HEIGHT, l);
    lineDraw(tri1.v3, tri1.v1, frag, WIDTH, HEIGHT, l);

}

void fillTriangle(tri tri1, float **frag, int WIDTH, int HEIGHT, float c){
    //sort points 
    vec3 p0, p1, p2;
    p0 = tri1.v1;
    p1 = tri1.v2;
    p2 = tri1.v3;

    if(p1.y < p0.y)
        swap(&p1, &p0);
    if(p2.y < p0.y)
        swap(&p2, &p0);
    if(p2.y < p1.y)
        swap(&p2, &p1);

    //scale points by a bit
    p0.x*=1.030625; p1.x*=1.030625; p2.x *=1.030625;
    p0.y*=1.030625; p1.y*=1.030625; p2.y *=1.030625;

    //
    int m, n, l;
    m = abs(p0.y - p1.y);
    n = abs(p1.y - p2.y);
    l = abs(p0.y - p2.y);

    float x01[m];
    float x12[n];
    float x02[l];

    //interpolate the points
    interpolate(x01, m, p0.y, p0.x, p1.y, p1.x);
    interpolate(x12, n, p1.y, p1.x, p2.y, p2.x);
    interpolate(x02, l, p0.y, p0.x, p2.y, p2.x);

    //testing
    
    
    //m--;    //remove last point
    int o = m + n;
    float x012[o];
    //concat points //causes corruption of points
    int i = 0, j = 0, k = 0;

    while(i < o){
        while(j < m)
            x012[i++] = x01[j++];
        while(k<n)
            x012[i++] = x12[k++];
    }


    //find left and right
    
    float* left = NULL, * right = NULL;
    int middle = l/2;
    
    if(x02[middle] < x012[middle]){
        left = x02;
        right = x012;
    }
    else{
        left = x012;
        right = x02;
    }
    //draw interpolated triangles
    lineDraw(p0, p1, frag, WIDTH, HEIGHT, c);
    lineDraw(p1, p2, frag, WIDTH, HEIGHT, c);
    lineDraw(p0, p2, frag, WIDTH, HEIGHT, c);

    for(int i = 0; i < o; i++){
        int len = abs(right[i] - left[i]);
        for(int j = 0; j <= len; j++){
            if(i < HEIGHT && j < WIDTH)
                frag[j + (int)left[i]][(i + (int) p0.y)] = c;
        }
    }
    
    
}



#endif