#ifndef FRAG_H
#define FRAG_H

#include <stdlib.h>
#include <stdio.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"


typedef struct{
    int flag;   //to see if current frag is computed
    vec3 coord;
    vec3 color
} Frag;


Frag * makeFrag(int WIDTH, int HEIGHT){
    Frag *frag = (Frag*) malloc(sizeof(Frag) * WIDTH * HEIGHT);  
    return frag;
}

int fragIndex(int x, int y, int WIDTH){
    return x + y * WIDTH;
}

Frag getFrag(Frag* frag, int x, int y, int WIDTH){
    return frag[fragIndex(x, y, WIDTH)];
}

void resetFrag(Frag* frag, int WIDTH, int HEIGHT){
    for(int i = 0; i < WIDTH * HEIGHT; i++){
        frag[i].flag = 0;
    }
}











#endif