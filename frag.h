#ifndef FRAG_H
#define FRAG_H

#include <stdlib.h>
#include <stdio.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"



float ** makeFrag(int WIDTH, int HEIGHT){
    float *values = (float*) calloc(WIDTH * HEIGHT, sizeof(float));
    float ** frag = (float**) malloc(WIDTH * sizeof(float*));

    for(int i = 0; i < WIDTH; i++){
        frag[i] = values + i*HEIGHT;
    }
    
    return frag;
}

void resetFrag(float** frag, int WIDTH, int HEIGHT){
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            frag[i][j] = 0.25;
        }
    }
}

void setFrag(vec3 points[], int point_len, float** frag, int WIDTH, int HEIGHT){
    for(int i = 0; i < point_len; i++){
        if(points[i].x >= 0 && points[i].x < WIDTH && points[i].y >= 0 && points[i].y < HEIGHT){
            frag[(int) points[i].x][(int) points[i].y] = points[i].l;
        }
    }
}

void fragTest(float** frag, int WIDTH, int HEIGHT){
    float slope = 0.1/(HEIGHT);
    float total = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            frag[j][i] -= total;
        }
        //total = slope * i;
        total += slope;
    }
}









#endif