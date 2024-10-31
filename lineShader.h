#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <stdlib.h>
#include "coords.h"
#include "vertex.h"
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

//returns 
float zGradient(vec3* vert1, vec3* vert2){
    int dx = vert2->x - vert1->x;
    float dz = vert2->z - vert1->z; //can be negative, doesnt matter
    return dz/dx;

}

//draw line in terms of x (bresenham algo)
void lineLow(vec3* vert1, vec3* vert2, vec3 points[], int n, int offset){
    int dx = vert2->x - vert1->x;
    int dy = vert2->y - vert1->y;
    int yi = 1;

    float z = zGradient(vert1, vert2);
    
    //array for our valid points in the line

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = (dy << 1) - dx;
    int y = vert1->y;
    for(int i = 0; i < n; i++){
        points[i + offset].x = i + vert1->x;
        points[i + offset].y = y;
        points[i + offset].z = i * z + vert1->z;
        if(D > 0){
            y += yi;
            D += (dy - dx) << 1;
        }
        else
            D += dy << 1;
    }
}

void lineHigh(vec3* vert1, vec3* vert2, vec3 points[], int n, int offset){
    int dx = vert2->x - vert1->x;
    int dy = vert2->y - vert1->y;
    int xi = 1;
    float z = zGradient(vert1, vert2);



    if(dx < 0){
        xi = -1;
        dx = -dx;
    }

    int D = (dx << 1) - dy;
    int x = vert1->x;

    //loop in reverse so it is sorted in ascending order
    for(int i = 0; i < n; i++){
        points[i + offset].x = x;
        points[i + offset].y = i + vert1->y;
        points[i + offset].z = i * z + vert1->z;
        if(D > 0){
            x += xi;
            D += (dx - dy) << 1;
        }
        else
            D += dx << 1;
    }


}

void lineDraw(vec3* vert1, vec3* vert2, vec3 points[], int n, int m, int offset){
    int x0 = vert1->x;
    int y0 = vert1->y;
    int x1 = vert2->x;
    int y1 = vert2->y;
    if(abs(y1 - y0) < abs(x1 - x0)){
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
int lineLen(vec3 vert[], int i, int j){
    int m = abs(vert[i].x - vert[j].x);
    int n = abs(vert[i].y - vert[j].y);
    int len = (m < n) ? n : m;
    return len + 1;

}
//length of a shape
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
int pointsLen(vec3 vert[], int indices[], int stride, int index_size){
    int points_len = 0;
    int shapes_n = index_size/stride;
    for(int i = 0; i < shapes_n; i++){
        points_len += shapeLen(vert, indices, stride, i);
    }
    return points_len;
}

void makeShape(vec3 vert[], int indices[], int stride, int offset, vec3 points[], int* point_offset){
    int start = offset * stride;
    int index1;
    int index2;
    int m;
    int n;
    //iterate through the stride and increase point offset
    for(int i = 0; i < stride - 1; i++, *(point_offset)++){
        index1 = indices[start + i];
        index2 = indices[start + i + 1];
        m = abs(vert[index1].x - vert[index2].x);
        n = abs(vert[index1].y - vert[index2].y);
        lineDraw(&vert[index1], &vert[index2], points, m, n, *(point_offset));
    }
    lineDraw(&vert[start], &vert[start + stride - 1], points, m, n, *(point_offset));
    *(point_offset)++;
}




#endif