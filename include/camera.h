#ifndef CAMERA_H
#define CAMERA_H
#include "vertex.h"

typedef struct{
    vec3 position;
    vec3 rotation;
    float focal_length;
} Camera;
#endif