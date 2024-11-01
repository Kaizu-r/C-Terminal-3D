#ifndef RENDERER_H
#define RENDERER
#include <stdio.h>
#include <stdlib.h>

#include "coords.h"


/*
    list of z value ascii
    
    .     :     <   |   X   Q   Z   *   &   @  
*/

//custom merge sort algo
void merge(vec3 vertices[], int i1, int j1, int i2, int j2){
    int start1, start2, k;
    vec3 temp[j2 + 1];
    
    start1 =  i1;
    start2 = i2;
    k = 0;
    //compare and copy to temp
    while(start1 <= j1 && start2 <= j2){
        //sort by y and x ascendingly, then descending for z
        if(vertices[start1].y < vertices[start2].y)
            temp[k++] = vertices[start1++]; 
        else if(vertices[start2].y < vertices[start1].y)
            temp[k++] = vertices[start2++]; 
        else if(vertices[start1].x < vertices[start2].x) //now we check for lesser x values if y is equal
            temp[k++] = vertices[start1++];
        else if(vertices[start2].x < vertices[start1].x)
            temp[k++] = vertices[start2++];
        else if(vertices[start1].z > vertices[start2].z) //now we check for z if x and y are equal
            temp[k++] = vertices[start1++];
        else
            temp[k++] = vertices[start2++];
    }

    //copy the rest to temp
    while(start1 <= j1){
        temp[k++] = vertices[start1++];
    }
    while(start2 <=j2){
        temp[k++] = vertices[start2++];
    }

    //copy back to array
    for(int i = i1, j = 0; i <= j2; i++, j++)
        vertices[i] = temp[j];

}

void mergesort(vec3 vertices[], int i, int j){
    if(i >= j)
        return;
    int mid = (i + j)/2;
    mergesort(vertices, i, mid);
    mergesort(vertices, mid+1, j);
    merge(vertices, i, mid, mid+1, j);
}

//implement our sorter
void sortVert(vec3 vert[], int n){
    mergesort(vert, 0, n-1);
}

void zPrint(float z){
    //textures sheesh
    if(z <= -0.8)
        printf(".");
    else if(z <= -0.6)
        printf(":");
    else if(z <= -0.4)
        printf("<");
    else if(z <= -0.2)
        printf("|");
    else if(z <= 0.0)
        printf("X");
    else if(z <= 0.2)
        printf("Q");
    else if(z <= 0.4)
        printf("Z");
    else if(z <= 0.6)
        printf("*");
    else if(z <= 0.8)
        printf("&");
    else if(z <= 1.0)
        printf("@");
}

//store only valid points in z
/*
    pseudo time!!!
    given this array of vec3 points
    [A, B, C, D, E, F, .........]
    recall that array is arranged in ascending y and x and descending z. this means that we move k until i at x and y != k at x and y
    increment j and then move i to k
   
*/
int zBuffer(vec3 vert[], int n){
    sortVert(vert, n);

    int i = 0;
    int j = 0;
    for(int k = 1; k < n; k++){
       if(vert[i].y != vert[k].y || vert[i].x != vert[k].x){ //store i to j
            vert[j++] = vert[i];
            i = k;
       }
    }
    vert[j++] = vert[i]; 
    return j;
}


//print our vertices
void render(vec3 vert[], int n){
    int line = 0;
    int space = 0;


    //start the printing
    line = vert[0].y - line;
    space = vert[0].x - space - 1;
    for(int i = 0; i < line; i++){
        printf("\n");
    }
    for(int i = 0; i < space; i++){
        printf(" ");
    }
    //do the real printing
    zPrint(vert[0].z);
    for(int i = 1; i < n; i++){
        //count number of lines needed for current vertex
        line = vert[i].y - vert[i-1].y;
        if(line == 0) //curr vertex and prev vertex are in the same line
            space = vert[i].x - vert[i -1].x - 1;
        else //curr vertex and prev vertex are NOT in the same lin
            space = vert[i].x - 1;
        for(int j = 0; j < line; j++){
            printf("\n");
        }
        for(int j = 0; j < space; j++){
            printf(" ");
        }

        zPrint(vert[i].z);
    }
    //print remaining spaces
    for(int i = 0; i < vert[n-1].y; i++)
        printf("\n");
}

#endif

