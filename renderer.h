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




float lightValue(vec3 col){
    //normalize col
    vec3 newCol = normalize(col);
    return (newCol.x + newCol.y + newCol.z)/3.0;
}

char toAscii(float z){

    //textures sheesh
    if(z > 1)
        return '@';
    else if(z > 0.9)
        return '&';
    else if(z > 0.8)
        return 'B';
    else if(z > 0.7)
        return 'Z';
    else if(z > 0.6)
        return 'Q';
    else if(z > 0.5)
        return 'X';
    else if(z > 0.4)
        return '|';
    else if(z > 0.3)
        return '<';
    else if(z > 0.2)
       return ':';
    else if(z > 0.1)
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
void render(Frag *frag, int WIDTH, int HEIGHT, char screen[]){
    int k = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(frag[fragIndex( j, i, WIDTH, HEIGHT)].flag){
                vec3 col = frag[fragIndex(j, i, WIDTH, HEIGHT)].color;
                screen[k++] = toAscii(col.z * 0.5);
            }
            else
                screen[k++] = ' ';
            
        }
        screen[k++] = '\n';
    }
    screen[k++] = '\0';
    printf(screen);
}

#endif

