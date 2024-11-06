#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <stdlib.h>
#include <stdio.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"
/*
    its pseudo time!!
    since two points make a line, we use a line algo of somekind to determine the points of the line
    Given points
    (x0, y0) -> (xn, yn)
    then the set of points between the two is
    {(x0, y0), (x1, y1).....(xn,yn)}


    since we print downwards then sidewards, then our algo must be in terms of x
    the set of points shall then be stored in a large array containing all other points, then sort them by ascending y value then print omega lul
    

*/



//draw line in terms of x (bresenham algo)
void lineLow(vec3* vert1, vec3* vert2, vec3 points[], int n, int offset){
    int dx = vert2->x - vert1->x;
    int dy = vert2->y - vert1->y;
    int dz = vert2->z - vert1->z;
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
    int y = vert1->y;
    int z = vert1->z;
    for(int i = 0; i <= n; i++){
        points[i + offset].x = i + vert1->x;
        points[i + offset].y = y;
        points[i + offset].z = z;
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

void lineHigh(vec3* vert1, vec3* vert2, vec3 points[], int n, int offset){
    int dx = vert2->x - vert1->x;
    int dy = vert2->y - vert1->y;
    int dz = vert2->z - vert1->z;
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
    int x = vert1->x;
    int z = vert1->z;

    //loop in reverse so it is sorted in ascending order
    for(int i = 0; i <= n; i++){
        points[i + offset].x = x;
        points[i + offset].y = i + vert1->y;
        points[i + offset].z = z;
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

void lineDraw(vec3* vert1, vec3* vert2, vec3 points[], int offset){
    int x0 = vert1->x;
    int y0 = vert1->y;
    int x1 = vert2->x;
    int y1 = vert2->y;
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
//length of a line
/* lineLen pseudo shit
    given points A and B, and dx is Ax - Bx annd dy is Ay - By,
    the number of points between A and B is determined by finding the greater number
    between abs of dx and abs of dy

*/
int lineLen(vec3 vert[], int i, int j){
    int m = abs(vert[i].x - vert[j].x);
    int n = abs(vert[i].y - vert[j].y);
    int len = (m < n) ? n : m;
    return len + 1;

}
//length of a shape
/*
    pseudo time
    Given a shape with stride n and points (X0, X1, ..., Xn),
    the perimter of the shape is the sum of all lines of point xi to xi + 1  until i = n, 
    then we get the length of the line of point x0 -> xn

*/
int shapeLen(vec3 vert[], int indices[], int stride, int offset){
    int vert_index = stride * offset;
    int shape_len = 0;
    for(int i = 0; i < stride - 1; i++){
        shape_len += lineLen(vert, indices[vert_index + i], indices[vert_index + i + 1]);
    }
    //connect last point with first point
    shape_len += lineLen(vert, indices[vert_index], indices[vert_index + stride - 1]);
    return shape_len;
}
//needed to find the size of our big array
/*
    pseudo time
    Given n indices with stride m, to get the number of points total,
    get the length of each shape with vertices m following the pattern provided by the indices,
    where the number of indices per shape is determined by the stride m, and the number of shape is
    n / m


*/
int pointsLen(vec3 vert[], int indices[], int stride, int index_size){
    int points_len = 0;
    int shapes_n = index_size/stride;
    for(int i = 0; i < shapes_n; i++){
        points_len += shapeLen(vert, indices, stride, i);
    }
    return points_len;
}

//downright the hardest thing to conceptualize
/*
    Consider the following vertices = {
    0, 0.5, 1,
    -0.5, 0, 1,
    0.5, 0, 1}
    and indices = {1, 0, 2}
    to cast our shape, we have to draw our lines from  1 -> 0, 0 -> 2, then 1->2

    lets say we have lines with length such that A = 5, B = 4, C = 5
    we have to store the points of A in an array of points points[pointsLen]

    to do this, we must get the length of each line using lineLen, use our lineDraw function such that
    our points are stored frome points[offset] to points[offset + lineLen] where offset
    is determined by an accumilator that increments for every lenght of the line that we have drawn

    someway somehow, we need to save the last offset so that the next shape can utilize the offset
    therefor we must pass a point for our point offset, which will determine the starting value of our offset

*/

//this is the bane of my existence
void makeShape(vec3 vert[], int indices[], vec3 points[], int stride, int offset, int* point_offset){
    int indices_start = offset * stride;
    int m;
    int n;
    int index1;
    int index2;
    int line_offset = *(point_offset); //starting index of starting point of a line
    for(int i = 0; i < stride - 1; i++){
        index1 = indices[indices_start + i];
        index2 = indices[indices_start + i + 1];
        m = abs(vert[index1].x - vert[index2].x);
        n = abs(vert[index1].y - vert[index2].y);
        lineDraw(&vert[index1], &vert[index2], points, line_offset);
        line_offset += lineLen(vert, index1, index2);
    }
    //draw our last line
    index1 = indices[indices_start];
    index2 = indices[indices_start + stride - 1];
    m = abs(vert[index1].x - vert[index2].x);
    n = abs(vert[index1].y - vert[index2].y);
    lineDraw(&vert[index1], &vert[index2], points, line_offset);
    line_offset += lineLen(vert, index1, index2);

    //mergesort(points, *point_offset, line_offset);
    //int total_y = points[line_offset].y - points[*(point_offset)].y;

   
    *point_offset = line_offset;
}



#endif