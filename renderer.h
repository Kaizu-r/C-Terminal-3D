#ifndef RENDERER_H
#define RENDERER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coords.h"
#include "utils.h"


/*
    list of z value ascii
    
    .     :     <   |   X   Q   Z   *   &   @  
*/



//implement our sorter
void sortVert(vec3 vert[], int n){
    mergesort(vert, 0, n-1);
}

void zPrint(float z, char* str){

    //textures sheesh
    if(z <= -0.8)
        strcat(str, "@");
    else if(z <= -0.6)
        strcat(str, "&");
    else if(z <= -0.4)
        strcat(str, "B");
    else if(z <= -0.2)
        strcat(str, "Z");
    else if(z <= 0.0)
        strcat(str, "Q");
    else if(z <= 0.2)
        strcat(str, "X");
    else if(z <= 0.4)
        strcat(str, "|");
    else if(z <= 0.6)
        strcat(str, "<");
    else if(z <= 0.8)
       strcat(str, ":");
    else
        strcat(str, ".");

}

//store only valid points in z
/*
    pseudo time!!!
    given this array of vec3 points
    [A, B, C, D, E, F, .........]
    recall that array is arranged in ascending y and x and descending z. this means that we move k until i at x and y != k at x and y
    increment j and then move i to k
   
*/
int zBuffer(vec3 vert[], int n, int width, int height){
    sortVert(vert, n);

    int i = 0;
    int j = 0;
    for(int k = 1; k < n; k++){
        if(vert[k].x >= 0 && vert[k].x <= width * 2){     
            if(vert[i].y != vert[k].y || vert[i].x != vert[k].x){ //store i to j
                vert[j] = vert[k];
                i = k;
                j++;
            }
        }
       
    }
    vert[j++] = vert[i]; 
    return j;
}


//print our vertices
void render(vec3 vert[], int n, int width, int height, char str[]){
    int line = 0;
    int space = 0;

    //start the printing
    line = vert[0].y;
    space = vert[0].x - 1;

    for(int i = 0; i < line; i++)
        strcat(str, "\n");
    for(int i = 0; i < space; i++)
        strcat(str, " ");
    
    //do the real printing
    if(vert[0].x >= 0 && vert[0].x <= width * 1.5 )
       zPrint(vert[0].z, str);

    for(int i = 1; i < n; i++){
        //count number of lines needed for current vertex
        //skip a vertex if its out of bounds
        while(vert[i -1].x < 0)
            i++;
        line = vert[i].y - vert[i-1].y;
        if(line == 0){ //curr vertex and prev vertex are in the same line
            space = vert[i].x - vert[i -1].x - 1;
        }
        else {//curr vertex and prev vertex are NOT in the same lin
            space = vert[i].x - 1;
        }

        for(int j = 0; j < line; j++){
            strcat(str, "\n");
        }
        for(int j = 0; j < space; j++){
            strcat(str, " ");
        }
        if(vert[i].x >= 0 && vert[i].x <= width << 1)
            zPrint(vert[i].z, str);
    };  
    for(int i = 0; i <= vert[n-1].y; i++){
       strcat(str, "\n");
    }
    strcat(str, "\0");
    printf(str);
}

#endif

