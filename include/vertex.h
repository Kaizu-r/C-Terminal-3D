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

typedef struct Vertex{
    vec3 position;
    vec2 uv;
    vec3 normal;
} Vertex;

//matrix structure
typedef struct mat{
    float matrix[3][3];
}mat3;
typedef struct triangle{
    Vertex v1;
    Vertex v2;
    Vertex v3; 
}tri;



#endif


