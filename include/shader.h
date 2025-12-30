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


void lineLow(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm);
void lineHigh(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm);
void lineDraw(vec3 vert1, vec3 vert2, List **list, int WIDTH, int HEIGHT, vec3 norm);
void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT, vec3 norm);
void fillTriangle(List **list, int WIDTH, int HEIGHT);
void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT);


//draw line in terms of x (bresenham algo)
void lineLow(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm){
    int dx = vert2.x - vert1.x;
    int dy = vert2.y - vert1.y;
    float dz = vert2.z - vert1.z;
    int yi = 1;

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = (dy << 1) - dx;
    int y = vert1.y;
    float z = vert1.z;
    float zstep = (dx == 0) ? 0.0f : dz / (float) dx;
    for(int i = 0; i <= n; i++){
        vec3* v_ptr = (vec3*) malloc(sizeof(vec3));
        if (!v_ptr) continue;
        *v_ptr = (vec3){ i + (int) vert1.x, (int) y, z, norm.x, norm.y, norm.z };
        pushBack(list, (void*) v_ptr);

        if(D > 0){
            y += yi;
            D += (dy - dx) << 1;
        } else
            D += dy << 1;
        z += zstep;
    }
}

void lineHigh(vec3 vert1, vec3 vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm){
    int dx = vert2.x - vert1.x;
    int dy = vert2.y - vert1.y;
    float dz = vert2.z - vert1.z;
    int xi = 1;

    if(dx < 0){
        xi = -1;
        dx = -dx;
    }

    int D = (dx << 1) - dy;
    int x = vert1.x;
    float z = vert1.z;
    float zstep = (dy == 0) ? 0.0f : dz / (float) dy;

    for(int i = 0; i <= n; i++){
        vec3* v_ptr = (vec3*) malloc(sizeof(vec3));
        if (!v_ptr) continue;
        *v_ptr = (vec3){ x, i + (int) vert1.y, (float) z, norm.x, norm.y, norm.z };
        
        pushBack(list, (void*) v_ptr);

        if(D > 0){
            x += xi;
            D += (dx - dy) << 1;
        } else
            D += dx << 1;
        z += zstep;
    }
}

void lineDraw(vec3 vert1, vec3 vert2, List **list, int WIDTH, int HEIGHT, vec3 norm){
    int x0 = vert1.x;
    int y0 = vert1.y;
    int x1 = vert2.x;
    int y1 = vert2.y;
    int n = abs(x0 - x1);
    int m = abs(y0 - y1);
    if(m < n){
        if(x0 > x1)
            lineLow(vert2, vert1, list, n, WIDTH, HEIGHT, norm);
        else
            lineLow(vert1, vert2, list, n, WIDTH, HEIGHT, norm);
    }
    else{
        if(y0 > y1)
            lineHigh(vert2, vert1, list, m, WIDTH, HEIGHT, norm);
        else
            lineHigh(vert1, vert2, list, m, WIDTH, HEIGHT, norm);
    }
}



void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT, vec3 norm){
    
    lineDraw(tri1.v1, tri1.v2, list, WIDTH, HEIGHT, norm);
    lineDraw(tri1.v2, tri1.v3, list, WIDTH, HEIGHT, norm);
    lineDraw(tri1.v3, tri1.v1, list, WIDTH, HEIGHT, norm);

}

void fillTriangle(List **list, int WIDTH, int HEIGHT){
    int len = (*list)->size;
    vec3 temp[len];
    /* copy contents */
    List_node* p = (*list)->front;
    int i = 0;
    while(p != NULL){
        temp[i++] = *((vec3* )(p->v));
        p = p->next;
    }

    mergesort3v(temp, 0, len - 1);

    for(int i = 0; i < len - 1; i++){
        if(temp[i].y == temp[i+1].y){
            int dx = temp[i+1].x - temp[i].x;
            float dz = temp[i+1].z - temp[i].z;
            float zstep = (dx == 0) ? 0: (dz/dx);

            for(int j = 1, k = zstep; j < dx; j++, k += zstep ){
                vec3 tmp = {temp[i].x + j, temp[i].y, temp[i].z + k, temp[i].nx, temp[i].ny, temp[i].nz};
                vec3* v_ptr = (vec3*) malloc(sizeof(vec3));
                if (!v_ptr) continue;
                *v_ptr = tmp;
                pushBack(list, (void*) v_ptr);
            }
        }
    }
}

void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT){
    List_node * p = list->front;
    while(p != NULL){
        vec3 v = *((vec3* )(p->v));

        vec2 coord = {v.x, v.y};
        Frag newFrag;
        newFrag.coord = v;
        
        vec3 color;
        vec3 light = {1, 1, 0};
        vec3 normal = {v.nx, v.ny, v.nz};
        float intensity = dot(normalize(light), normalize(normal));
        color.z = intensity;



        newFrag.color = color;

        int index = fragIndex(coord.x, coord.y, WIDTH, HEIGHT);
        if(index < 0){
           p = p->next;
           continue; 
        }
        if(frag[index].flag){
            frag[index] = (v.z < frag[index].color.z) ? newFrag : frag[index];
        }
        else{
            frag[index] = newFrag;
        }
        frag[index].flag = 1;
        p = p->next;
    }
}




#endif