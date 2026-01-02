#ifndef LINE_SHADER_H
#define LINE_SHADER_H
#include <math.h>
#include "list.h"
#include "frag.h"


void lineLow(Vertex vert1, Vertex vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm);
void lineHigh(Vertex vert1, Vertex vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm);
void lineDraw(Vertex vert1, Vertex vert2, List **list, int WIDTH, int HEIGHT, vec3 norm);
void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT, vec3 norm);
void fillTriangle(List **list, int WIDTH, int HEIGHT);
void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT);


//draw line in terms of x (bresenham algo)
void lineLow(Vertex vert1, Vertex vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm){
    int dx = vert2.position.x - vert1.position.x;
    int dy = vert2.position.y - vert1.position.y;
    float dz = vert2.position.z - vert1.position.z;
    int yi = 1;

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = (dy << 1) - dx;
    int y = vert1.position.y;
    float z = vert1.position.z;

    //interpolate z value for dept
    float zstep = (dx == 0) ? 0.0f : dz / (float) dx;
    for(int i = 0; i <= n; i++){
        Vertex* v_ptr = (Vertex*) malloc(sizeof(Vertex));
        if (!v_ptr) continue;
        //create vertex with normal info
        v_ptr->position = (vec3){ i + (int) vert1.position.x, (int) y, z };
        v_ptr->normal = norm;
        v_ptr->uv = vert1.uv;
        pushBack(list, (void*) v_ptr);

        if(D > 0){
            y += yi;
            D += (dy - dx) << 1;
        } else
            D += dy << 1;
        z += zstep;
    }
}

void lineHigh(Vertex vert1, Vertex vert2, List **list, int n, int WIDTH, int HEIGHT, vec3 norm){
    int dx = vert2.position.x - vert1.position.x;
    int dy = vert2.position.y - vert1.position.y;
    float dz = vert2.position.z - vert1.position.z;
    int xi = 1;

    if(dx < 0){
        xi = -1;
        dx = -dx;
    }

    int D = (dx << 1) - dy;
    int x = vert1.position.x;
    float z = vert1.position.z;

    //interpolate z value for depth
    float zstep = (dy == 0) ? 0.0f : dz / (float) dy;

    for(int i = 0; i <= n; i++){
        Vertex* v_ptr = (Vertex*) malloc(sizeof(Vertex));
        if (!v_ptr) continue;

        //create vertex with normal info
        v_ptr->position = (vec3){ x, i + (int) vert1.position.y, (float) z };
        v_ptr->normal = norm;
        v_ptr->uv = vert1.uv;
        
        pushBack(list, (void*) v_ptr);

        if(D > 0){
            x += xi;
            D += (dx - dy) << 1;
        } else
            D += dx << 1;
        z += zstep;
    }
}

//general line draw function
void lineDraw(Vertex vert1, Vertex vert2, List **list, int WIDTH, int HEIGHT, vec3 norm){
    int x0 = vert1.position.x;
    int y0 = vert1.position.y;
    int x1 = vert2.position.x;
    int y1 = vert2.position.y;
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


//draw each triangle edge
void drawTriangle(tri tri1, List **list, int WIDTH, int HEIGHT, vec3 norm){
    
    lineDraw(tri1.v1, tri1.v2, list, WIDTH, HEIGHT, norm);
    lineDraw(tri1.v2, tri1.v3, list, WIDTH, HEIGHT, norm);
    lineDraw(tri1.v3, tri1.v1, list, WIDTH, HEIGHT, norm);

}

//rasterze the triangle
void fillTriangle(List **list, int WIDTH, int HEIGHT){
    int len = (*list)->size;
    Vertex temp[len];
    /* copy contents */
    List_node* p = (*list)->front;
    int i = 0;
    while(p != NULL){
        temp[i++] = *((Vertex* )(p->v));
        p = p->next;
    }

    //sort by y, x, then z
    mergesort3v(temp, 0, len - 1);

    for(int i = 0; i < len - 1; i++){
        if(temp[i].position.y == temp[i+1].position.y){
            //check for x gap
            int dx = temp[i+1].position.x - temp[i].position.x;
            //interpolate z value
            float dz = temp[i+1].position.z - temp[i].position.z;
            float zstep = (dx == 0) ? 0: (dz/dx);

            //fill x gap
            for(int j = 1, k = zstep; j < dx; j++, k += zstep ){
                Vertex tmp = temp[i];
                tmp.position.x = temp[i].position.x + j;
                tmp.position.y = temp[i].position.y;
                tmp.position.z = temp[i].position.z + k;
                Vertex* v_ptr = (Vertex*) malloc(sizeof(Vertex));
                if (!v_ptr) continue;
                *v_ptr = tmp;
                pushBack(list, (void*) v_ptr);
            }
        }
    }
}

//place fragment in frag buffer
void placeFrag(Frag* frag, List *list, int WIDTH, int HEIGHT){
    List_node * p = list->front;
    while(p != NULL){
        Vertex v = *((Vertex* )(p->v));

        vec2 coord = {v.position.x, v.position.y};
        Frag newFrag;
        newFrag.flag = true;
        newFrag.coord = v.position;
        
        vec3 color;
        vec3 light = {1, 1, 0};
        vec3 normal = v.normal;

        //basic lambertian shading
        float intensity = dot(normalize(light), normalize(normal));
        newFrag.depth = v.position.z;
        color.z = intensity;

        newFrag.normal = v.normal;

        newFrag.color = color;

        int index = fragIndex(coord.x, coord.y, WIDTH, HEIGHT);
        if(index < 0){
           p = p->next;
           continue; 
        }
        if(frag[index].flag){
            //depth test
            frag[index] = (v.position.z < frag[index].depth) ? newFrag : frag[index];
        }
        else{
            frag[index] = newFrag;
        }
        frag[index].flag = true;
        p = p->next;
    }
}




#endif