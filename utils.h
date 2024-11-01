#ifndef PI
#define PI 3.14159
#endif
#ifndef UTILS_H
#define UTILS_H


#include <stdlib.h>
#include "vertex.h"
#include "math.h"


//degrees to radians convertion
float rad(int deg){
    float y = deg * PI/180;
    return y;
}

//rotation matrixes (keep in mind our z is the other way around so multiply the 3rd column by -1)
mat3 rotateX(float radians){
    mat3 rotX = {
        1, 0, 0,
        0,  cos(radians), -sin(radians),
        0, sin(radians), cos(radians)
    };
    return rotX;
}

mat3 rotateY(float radians){
    mat3 rotY = {
        cos(radians), 0, sin(radians),
        0, 1, 0, 
        -sin(radians), 0, cos(radians)
    };
    return rotY;
}

mat3 rotateZ(float radians){
    mat3 rotZ = {
        cos(radians), -sin(radians), 0,
        sin(radians), cos(radians), 0,
        0, 0, 1
    };
    return rotZ;
}

//matrix multiplication
mat3 matMultiply(mat3 mat1, mat3 mat2){
    mat3 product;
    for(int i = 0; i < 3; i++){ //rows
        for(int j = 0; j < 3; j++){ //column
            product.matrix[i][j] = mat1.matrix[i][0] * mat2.matrix[0][j] + mat1.matrix[i][1] * mat2.matrix[1][j] + mat1.matrix[i][2] * mat2.matrix[2][j];
        }
    }
    return product;
}

//Transformation matrix
mat3 matTransform(mat3 matx, mat3 maty, mat3 matz){
    mat3 transform = matMultiply(matz, maty);
    transform = matMultiply(transform, matx);
    return transform;
}

//responsible for rotating our vertices based on model transfomration
void model(vec3 vert[], int degX, int degY, int degZ){
    float radX = rad(degX);
    float radY = rad(degY);
    float radZ = rad(degZ);

    //get our rotation matrixes
    mat3 rotX = rotateX(radX);
    mat3 rotY = rotateY(radY);
    mat3 rotZ = rotateZ(radZ);

    //get our transformation matrix
    mat3 transform = matTransform(rotX, rotY, rotZ);

    //transform each vertex
    int vertNum = sizeof(vert)/sizeof(vec3);
    for(int i = 0; i < vertNum; i++){
        vert[i].x = vert[i].x * transform.matrix[0][0] + vert[i].y * transform.matrix[0][1] + vert[i].z * transform.matrix[0][2];
        vert[i].y = vert[i].x * transform.matrix[1][0] + vert[i].y * transform.matrix[1][1] + vert[i].z * transform.matrix[1][2];
        vert[i].z = vert[i].x * transform.matrix[2][0] + vert[i].y * transform.matrix[2][1] + vert[i].z * transform.matrix[2][2];
    }
}

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

//returns slope of z on either x or y depending on mode
float zGradient(vec3* vert1, vec3* vert2, int mode){
    
    int dx = vert2->x - vert1->x;
    int dy = vert2->y - vert1->y;
    float dz = vert2->z - vert1->z; //can be negative, doesnt matter
    
    if(mode == 0)
        return dz/dy;
    return dz/dx;

}

//matrix transformations
















#endif