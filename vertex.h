#ifndef VERTEX_H
#define VERTEX_H
#include <stdlib.h>

//vector 
typedef struct vector3{
    float x;
    float y;
    float z;
} vec3;

typedef struct vector2{
    float x;
    float y;
} vec2;
//matrix structure
typedef struct mat{
    float matrix[3][3];
}mat3;
typedef struct triangle{
    vec3 v1;
    vec3 v2;
    vec3 v3; 
}tri;

typedef struct Mesh{
    vec3* vert;
    int* index;
}mesh;



#endif


