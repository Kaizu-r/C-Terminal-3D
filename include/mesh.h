#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Vertex * vertices;
    int * indices;
    vec3 rotation;
    vec3 position;
    int vertexCount;
    int indexCount;
    float scale;
} Mesh;

void initMesh(Mesh* mesh, Vertex* vertices, int* indices, vec3 rotation, vec3 position, int vertexCount, int indexCount) {
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->rotation = rotation;
    mesh->position = position;
    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;
}

int loadMesh(Mesh* mesh, const char* filepath){

    //open file
    FILE *f = fopen(filepath, "r");
    if(f == NULL){
        printf("Failed to open mesh file: %s\n", filepath);
        return -1;
    }
    
    printf("Loading mesh from: %s\n", filepath);

    //start counting vertices and indices
    int vertexCount = 0;
    int indexCount = 0;

    char line[128];
    while(fgets(line, sizeof(line), f)){
        if(line[0] == 'v' && line[1] == ' '){
            vertexCount++;
        } else if(line[0] == 'f' && line[1] == ' '){
            //each face line has 3 indices
            indexCount += 3;
        }
    }

    //allocate memory
    mesh->vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    mesh->indices = (int*)malloc(sizeof(int) * indexCount);

    //reset file pointer
    fseek(f, 0, SEEK_SET);
    int vIndex = 0;
    int iIndex = 0;

    while(fgets(line, sizeof(line), f)){
        if(line[0] == 'v' && line[1] == ' '){
            sscanf(line, "v %f %f %f", 
                &mesh->vertices[vIndex].position.x, 
                &mesh->vertices[vIndex].position.y, 
                &mesh->vertices[vIndex].position.z);
            // Flip Y-axis to correct orientation
            mesh->vertices[vIndex].position.y = -mesh->vertices[vIndex].position.y;
            mesh->vertices[vIndex].uv = (vec2){0.0f, 0.0f};
            mesh->vertices[vIndex].normal = (vec3){0.0f, 0.0f, 1.0f};
            vIndex++;
        }
        else if(line[0] == 'f' && line[1] == ' '){
            int v1, v2, v3;
            sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            //OBJ indices are 1-based, convert to 0-based
            mesh->indices[iIndex++] = v1 - 1;
            mesh->indices[iIndex++] = v2 - 1;
            mesh->indices[iIndex++] = v3 - 1;
        }
    }

    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;
    
    printf("Loaded %d vertices and %d indices\n", vertexCount, indexCount);
    
    fclose(f);
    return 0;

}

void freeMesh(Mesh* mesh) {
    if (mesh->vertices) {
        free(mesh->vertices);
        mesh->vertices = NULL;
    }
    if (mesh->indices) {
        free(mesh->indices);
        mesh->indices = NULL;
    }
}

void setMeshPosition(Mesh* mesh, vec3 position) {
    mesh->position = position;
}
void setMeshRotation(Mesh* mesh, vec3 rotation) {
    mesh->rotation = rotation;
}
#endif