#ifndef FRAG_H
#define FRAG_H

#include <stdlib.h>
#include <stdio.h>
#include "coords.h"
#include "vertex.h"
#include "utils.h"



void toFrag(vec3 point[], int point_len, float *frag[], int WIDTH, int HEIGHT){
    for(int i = 0; i < point_len; i++){
        //copy to frag at index (x, y) if it is bound by the screen
        if(point[i].x >= 0 && point[i].y >= 0 && point[i].x < WIDTH && point[i].y < HEIGHT){
            frag[ (int) point[i].x][ (int) point[i].y] = point[i].l; // transfer lighting value
        }
    }
}











#endif