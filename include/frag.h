#ifndef FRAG_H
#define FRAG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"


typedef struct{
    bool flag;   //to see if current frag is computed
    vec3 coord;
    vec3 color;
    float depth;
    vec3 normal;
} Frag;

Frag * makeFrag(int WIDTH, int HEIGHT);
int fragIndex(int x, int y, int WIDTH, int HEIGHT);
Frag getFrag(Frag* frag, int x, int y, int WIDTH, int HEIGHT);
void resetFrag(Frag* frag, int WIDTH, int HEIGHT);

Frag * makeFrag(int WIDTH, int HEIGHT){
    Frag *frag = (Frag*) malloc(sizeof(Frag) * WIDTH * HEIGHT);  
    for(int i = 0; i < WIDTH * HEIGHT; i++){
        frag[i].flag = false;
    }
    return frag;
}

int fragIndex(int x, int y, int WIDTH, int HEIGHT){
    if(x < 0 | x >= WIDTH | y < 0 | y >= HEIGHT)
        return -1;
    return x + y * WIDTH;
}

Frag getFrag(Frag* frag, int x, int y, int WIDTH, int HEIGHT){
    return frag[fragIndex(x, y, WIDTH, HEIGHT)];
}

void resetFrag(Frag* frag, int WIDTH, int HEIGHT){
    for(int i = 0; i < WIDTH * HEIGHT; i++){
        frag[i].flag = false;
    }
}











#endif