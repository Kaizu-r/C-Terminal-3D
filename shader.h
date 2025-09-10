#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"
#include "list.h"
#include "frag.h"


//draw line in terms of x (bresenham algo)
void lineLow(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, float l){
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
        && y < HEIGHT && i + (int)vert1.x < WIDTH){
            vec3 v = {i + (int) vert1.x, (int) y, z};
            pushBack(list, v);
        }
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

void lineHigh(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, float l){
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
        && x < WIDTH && i + (int) vert1.y < HEIGHT){
            vec3 v = {x, i + (int) vert1.y, (float) z};
            pushBack(list, v);
        }
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

void lineDraw(vec3 vert1, vec3 vert2, List **list, int WIDTH, int HEIGHT, float l){
    int x0 = vert1.x;
    int y0 = vert1.y;
    int x1 = vert2.x;
    int y1 = vert2.y;
    int n = abs(x0 - x1);
    int m = abs(y0 - y1);
    if(m < n){
        if(x0 > x1)
            lineLow(vert2, vert1, list, n, WIDTH, HEIGHT, l);
        else
            lineLow(vert1, vert2, list, n, WIDTH, HEIGHT, l);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, list, m, WIDTH, HEIGHT, l);
        else
            lineHigh(vert1, vert2, list, m, WIDTH, HEIGHT, l);
    }
}


//no longer usable





void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT, float l){
    
    lineDraw(tri1.v1, tri1.v2, list, WIDTH, HEIGHT, l);
    lineDraw(tri1.v2, tri1.v3, list, WIDTH, HEIGHT, l);
    lineDraw(tri1.v3, tri1.v1, list, WIDTH, HEIGHT, l);

}

void fillTriangle(List **list, int WIDTH, int HEIGHT){
    int len = (*list)->size;
    vec3 temp[len];
    //copy contents
    List_node* p = (*list)->front;
    int i = 0;
    while(p != NULL){
        temp[i++] = p->v;
        p = p->next;
    }
    

    //sort temp array
    mergesort3v(temp, 0, len - 1);

    
    for(int i = 0; i < len - 1; i++){
        
        if(temp[i].y == temp[i+1].y){
            int dx = temp[i+1].x - temp[i].x;
            float dz = temp[i+1].z - temp[i].z;
            float zstep = (dx == 0) ? 0: (dz/dx);

            for(int j = 1, k = zstep; j < dx; j++, k += zstep ){
                vec3 v = {temp[i].x + j, temp[i].y, temp[i].z + k};
                
                pushBack(list, v);
            }
        }
    }
    

}

void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT){
    List_node * p = list->front;
    while(p != NULL){
        vec2 coord = {p->v.x, p->v.y};
        Frag newFrag;
        newFrag.coord = p->v;
        vec3 color = {0, 0, p->v.z};
        newFrag.color = color;

        int index = fragIndex(coord.x, coord.y, WIDTH, HEIGHT);
        if(index < 0){
           p = p->next;
           continue; 
        }
        if(frag[index].flag){
            frag[index] = (p->v.z < frag[index].color.z) ? newFrag : frag[index];
        }
        else{
            frag[index] = newFrag;
        }
        frag[index].flag = 1;
        p = p->next;
    }
}


/*
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
    */



#endif