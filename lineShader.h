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
void lineLow(vec3* vert1, vec3* vert2, vec3 points[], int n){
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
        points[i].x = i + vert1->x;
        points[i].y = y;
        points[i].z = i * z + vert1->z;
        if(D > 0){
            y += yi;
            D += (dy - dx) << 1;
        }
        else
            D += dy << 1;
    }
}

void lineHigh(vec3* vert1, vec3* vert2, vec3 points[], int n){
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
        points[i].x = x;
        points[i].y = i + vert1->y;
        points[i].z = i * z + vert1->z;
        if(D > 0){
            x += xi;
            D += (dx - dy) << 1;
        }
        else
            D += dx << 1;
    }


}

void lineDraw(vec3* vert1, vec3* vert2, vec3 points[], int n, int m){
    int x0 = vert1->x;
    int y0 = vert1->y;
    int x1 = vert2->x;
    int y1 = vert2->y;
    if(abs(y1 - y0) < abs(x1 - x0)){
        if(x0 > x1)
            lineLow(vert2, vert1, points, n);
        else
            lineLow(vert1, vert2, points, n);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, points, m);
        else
            lineHigh(vert1, vert2, points, m);
    }
}






#endif