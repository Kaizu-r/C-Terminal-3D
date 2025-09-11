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


void lineLow(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT);
void lineHigh(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT);
void lineDraw(vec3 vert1, vec3 vert2, List **list, int WIDTH, int HEIGHT);
void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT);
void fillTriangle(List **list, int WIDTH, int HEIGHT);
void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT);


//draw line in terms of x (bresenham algo)
void lineLow(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT){
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

void lineHigh(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT){
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

void lineDraw(vec3 vert1, vec3 vert2, List **list, int WIDTH, int HEIGHT){
    int x0 = vert1.x;
    int y0 = vert1.y;
    int x1 = vert2.x;
    int y1 = vert2.y;
    int n = abs(x0 - x1);
    int m = abs(y0 - y1);
    if(m < n){
        if(x0 > x1)
            lineLow(vert2, vert1, list, n, WIDTH, HEIGHT);
        else
            lineLow(vert1, vert2, list, n, WIDTH, HEIGHT);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, list, m, WIDTH, HEIGHT);
        else
            lineHigh(vert1, vert2, list, m, WIDTH, HEIGHT);
    }
}


//no longer usable





void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT){
    
    lineDraw(tri1.v1, tri1.v2, list, WIDTH, HEIGHT);
    lineDraw(tri1.v2, tri1.v3, list, WIDTH, HEIGHT);
    lineDraw(tri1.v3, tri1.v1, list, WIDTH, HEIGHT);

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




#endif