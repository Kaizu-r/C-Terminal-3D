#ifndef MESH_H
#define MESH_H

#include "vertex.h"

typedef struct {
    vec3 * vertices;
    int * indices;
    vec3 rotation;
} Mesh;


#endif