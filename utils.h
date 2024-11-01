#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include "vertex.h"

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


















#endif