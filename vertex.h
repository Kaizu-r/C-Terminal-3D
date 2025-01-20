#ifndef VERTEX_H
#define VERTEX_H
#include <stdlib.h>

//vector 
typedef struct vector3{
    float x;
    float y;
    float z;
    float l;
} vec3;

typedef struct vector2{
    float x;
    float y;
} vec2;
//matrix structure
typedef struct mat{
    float matrix[3][3];
}mat3;



#endif


