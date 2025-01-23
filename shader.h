#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"


//draw line in terms of x (bresenham algo)
void lineLow(vec3 vert1, vec3 vert2, vec3 points[], int n, int offset){
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
        points[i + offset].x = i + vert1.x;
        points[i + offset].y = y;
        points[i + offset].z = z;
        points[i + offset].l = vert1.l;
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

void lineHigh(vec3 vert1, vec3 vert2, vec3 points[], int n, int offset){
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
        points[i + offset].x = x;
        points[i + offset].y = i + vert1.y;
        points[i + offset].z = z;
        points[i + offset].l = vert1.l;
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

void lineDraw(vec3 vert1, vec3 vert2, vec3 points[], int offset){
    int x0 = vert1.x;
    int y0 = vert1.y;
    int x1 = vert2.x;
    int y1 = vert2.y;
    int n = abs(x0 - x1);
    int m = abs(y0 - y1);
    if(m < n){
        if(x0 > x1)
            lineLow(vert2, vert1, points, n, offset);
        else
            lineLow(vert1, vert2, points, n, offset);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, points, m, offset);
        else
            lineHigh(vert1, vert2, points, m, offset);
    }
}

int lineLen(vec3 vert1, vec3 vert2){
    int m = abs(vert1.x - vert2.x);
    int n = abs(vert1.y - vert2.y);
    int len = (m < n) ? n : m;
    return len + 1;

}

int shapeLen(vec3 vert[], int indices[], int stride, int offset){
    int vert_index = stride * offset;
    int shape_len = 0;
    //area of a our triangle
    int a = lineLen(vert[indices[vert_index]], vert[indices[vert_index + 1]]);
    int b = lineLen(vert[indices[vert_index + 1]], vert[indices[vert_index + 2]]);
    int c = lineLen(vert[indices[vert_index]], vert[indices[vert_index + 2]]);

    float s = (a + b + c)/2;



    //big approx to give headroom
    shape_len = sqrt((s * (s - a) * (s - b) * (s - c))) * 2;
    return shape_len;
}

int pointsLen(vec3 vert[], int indices[], int stride, int index_size){
    int points_len = 0;
    int shapes_n = index_size/stride;
    for(int i = 0; i < shapes_n; i++){
        points_len += shapeLen(vert, indices, stride, i);
    }
    printf("%d\n", points_len);
    return points_len;
}

//ray traced shadows
void emit_light(vec3 points[], int point_len, vec3 world[], int world_len, vec3 light, float res, int distance, int WIDTH, int HEIGHT){

    light = toTerminal(&light, WIDTH, HEIGHT);
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
            if(world_points[(int) world[i].x][(int) world[i].y][(int) world[i].z] == -1){  //if still fre
                world_points[(int) world[i].x][(int) world[i].y][(int) world[i].z] = i;
            }
        }
    }

    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            if((screen_points[i][j] != -1)){//skip step if no point exists
                vec3 dir;
                int index = screen_points[i][j];
                vec3 curr = points[index];
                dir = v_angle(points[index], light);
                
                vec3 orig = curr;

                int k = 0;
                
                while(1){
                    k++;
                    
                    curr.x =precision(orig.x + k * dir.x, res);
                    curr.y =precision(orig.y + k * dir.y, res);
                    curr.z =precision(orig.z + k * dir.z, res);
                    int x_index = (int) curr.x;
                    int y_index = (int) curr.y;
                    int z_index = (int) curr.z;
                    int modified_z = z_index + distance/2.0;

                    
                    if(x_index >= WIDTH || y_index >= HEIGHT || modified_z < 0 || modified_z > distance){ //out of bounds, therefore no point
                        break;
                    }
                    
                    if(world_points[x_index][y_index][modified_z] != -1){   //valid point
                        points[index].l = 0.355;
                        //printf("x");
                        break;
                    }
                    
                    
                }
            }
            



        }
    }




}

void raster(vec3 points[], int start, int end, int *point_offset){
    mergesort(points, start, end); //sort first
    for(int i = start; i <= end; i++){
        if(points[i].y == points[i + 1].y){
            lineDraw(points[i], points[i + 1], points, *point_offset);
            *point_offset += lineLen(points[i], points[i + 1]);

        }

    }
}

//not working rip
void makeGround(vec3 points[], int point_len, vec3 scene[], int scene_points, int HEIGHT, int WIDTH, int FAR){
    int i = 0;
    int j = 0;
    vec3 curr = {10, HEIGHT/2, FAR};
    
    mergesort(points, 0, point_len - 1);
    while(points[i].y <= HEIGHT/2)
        scene[j++] = points[i++];
    while(i < point_len){
        if(points[i].x != curr.x && points[i].y != curr.y && points[i].z != curr.z){
            scene[j++] = curr;
        }
        else
            scene[j++] = points[i];
        if(curr.x >= WIDTH){
            curr.y--;
            curr.z--;
            curr.x = 10;
        }
        else{
            curr.x++;
        }
        i++;
    }

    //mergesort(scene, 0, scene_points - 1);
}


void makeShape(vec3 vert[], int indices[], vec3 points[], int stride, int offset, int* point_offset){
    int indices_start = offset * stride;
    int index1;
    int index2;
    int start = *(point_offset);
    int line_offset = *(point_offset); //starting index of starting point of a line
    for(int i = 0; i < stride - 1; i++){
        index1 = indices[indices_start + i];
        index2 = indices[indices_start + i + 1];
        
        lineDraw(vert[index1], vert[index2], points, line_offset);
        line_offset += lineLen(vert[index1], vert[index2]);
    }
    //draw our last line
    index1 = indices[indices_start];
    index2 = indices[indices_start + stride - 1];
    lineDraw(vert[index1], vert[index2], points, line_offset);
    line_offset += lineLen(vert[index1], vert[index2]);

    //mergesort(points, *point_offset, line_offset);
    //int total_y = points[line_offset].y - points[*(point_offset)].y;

   
    *point_offset = line_offset;
    raster(points, start, *point_offset, point_offset);
}




#endif