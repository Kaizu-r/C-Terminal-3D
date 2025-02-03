#ifndef RENDERER_H
#define RENDERER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coords.h"
#include "utils.h"
#include "vertex.h"


/*
    list of z value ascii
    
    .     :     <   |   X   Q   Z   *   &   @  
*/



//implement our sorter
void sortVert(vec3 vert[], int n){
    mergesort(vert, 0, n-1);
}



char zPrint(float z){

    //textures sheesh
    if(z < -0.8)
        return '@';
    else if(z < -0.6)
        return '&';
    else if(z < -0.4)
        return 'B';
    else if(z < -0.2)
        return 'Z';
    else if(z < -0.1)
        return 'Q';
    else if(z < 0.1)
        return 'X';
    else if(z < 0.2)
        return '|';
    else if(z < 0.4)
        return '<';
    else if(z < 0.6)
       return ':';
    else if(z < 0.8)
        return '\'';
    else
        return ' ';

}

//store only valid points in z
/*
    pseudo time!!!
    given this array of vec3 points
    [A, B, C, D, E, F, .........]
    recall that array is arranged in ascending y and x and descending z. this means that we move k until i at x and y != k at x and y
    increment j and then move i to k
   
*/




//more in line with traditional rendering
void render(float **frag, int WIDTH, int HEIGHT, char screen[]){
    int k = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            screen[k++] = zPrint(frag[j][i]);
        }
        screen[k++] = '\n';
    }
    screen[k++] = '\0';
    printf(screen);
}

#endif

