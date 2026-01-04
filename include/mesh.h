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
    int textureCount = 0;
    int normalCount = 0;

    char line[128];
    while(fgets(line, sizeof(line), f)){
        if(line[0] == 'v' && line[1] == ' '){
            vertexCount++;
        } else if(line[0] == 'f' && line[1] == ' '){
            //count vertices in face by counting spaces
            int space_count = 0;
            for(int i = 0; line[i] != '\0' && line[i] != '\n'; i++){
                if(line[i] == ' ') space_count++;
            }
            // "f v1 v2 v3" has 3 spaces, "f v1 v2 v3 v4" has 4 spaces
            // Quads get split into 2 triangles (6 indices)
            if(space_count >= 4){
                indexCount += 6;  // quad
            } else {
                indexCount += 3;  // triangle
            }
        }
        else if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
            textureCount++;
        }
        else if(line[0] == 'v' && line[1] == 'n' && line[2] == ' '){
            normalCount++;
        }
    }

    

    //allocate memory
    mesh->vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    mesh->indices = (int*)malloc(sizeof(int) * indexCount);
    vec2 * temp_uvs = (textureCount > 0) ? (vec2*)malloc(sizeof(vec2) * textureCount) : NULL;
    vec3 * temp_normals = (normalCount > 0) ? (vec3*)malloc(sizeof(vec3) * normalCount) : NULL;
    if(mesh->vertices == NULL || mesh->indices == NULL){
        printf("Failed to allocate memory for mesh\n");
        fclose(f);
        return -1;
    }
    
    //initialize vertices with default values if no texture/normal data
    for(int i = 0; i < vertexCount; i++){
        mesh->vertices[i].uv = (vec2){0.0f, 0.0f};
        mesh->vertices[i].normal = (vec3){0.0f, 1.0f, 0.0f};
    }


    //reset file pointer
    fseek(f, 0, SEEK_SET);
    int vIndex = 0;
    int iIndex = 0;
    int uvIndex = 0;
    int nIndex = 0;

    while(fgets(line, sizeof(line), f)){
        
        if(line[0] == 'v' && line[1] == ' '){
            sscanf(line, "v %f %f %f", 
                &mesh->vertices[vIndex].position.x, 
                &mesh->vertices[vIndex].position.y, 
                &mesh->vertices[vIndex].position.z);
            // Flip Y-axis to correct orientation
            mesh->vertices[vIndex].position.y = -mesh->vertices[vIndex].position.y;
            vIndex++;
        }
        else if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
            sscanf(line, "vt %f %f", 
                &temp_uvs[uvIndex].x, 
                &temp_uvs[uvIndex].y);
            // Flip V coordinate
            temp_uvs[uvIndex].y = 1.0f - temp_uvs[uvIndex].y;
            uvIndex++;
        }
        else if(line[0] == 'v' && line[1] == 'n' && line[2] == ' '){
            sscanf(line, "vn %f %f %f", 
                &temp_normals[nIndex].x, 
                &temp_normals[nIndex].y, 
                &temp_normals[nIndex].z);
            nIndex++;
        }
        else if(line[0] == 'f' && line[1] == ' '){
            //index holders
            int v1[3] = {0}, v2[3] = {0}, v3[3] = {0}, v4[3] = {0};
            int parsed = 0;
            int vertex_count = 0;
            
            // First, try to detect if it's a quad (4 vertices) or triangle (3 vertices)
            // by counting spaces
            int space_count = 0;
            for(int i = 0; line[i] != '\0' && line[i] != '\n'; i++){
                if(line[i] == ' ') space_count++;
            }
            
            // space_count includes the "f " part, so 4+ spaces = quad, 3 = triangle
            if(space_count >= 4){
                // Try quad parsing formats
                parsed = sscanf(line, "f %d %d %d %d", &v1[0], &v2[0], &v3[0], &v4[0]);
                if(parsed != 4){
                    parsed = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
                        &v1[0], &v1[1], &v1[2],
                        &v2[0], &v2[1], &v2[2],
                        &v3[0], &v3[1], &v3[2],
                        &v4[0], &v4[1], &v4[2]);
                }
                if(parsed != 12 && parsed != 4){
                    parsed = sscanf(line, "f %d/%d %d/%d %d/%d %d/%d", 
                        &v1[0], &v1[1],
                        &v2[0], &v2[1],
                        &v3[0], &v3[1],
                        &v4[0], &v4[1]);
                }
                if(parsed != 8 && parsed != 4 && parsed != 12){
                    parsed = sscanf(line, "f %d//%d %d//%d %d//%d %d//%d", 
                        &v1[0], &v1[2],
                        &v2[0], &v2[2],
                        &v3[0], &v3[2],
                        &v4[0], &v4[2]);
                }
            } else {
                // Try triangle parsing formats
                parsed = sscanf(line, "f %d %d %d", &v1[0], &v2[0], &v3[0]);
                if(parsed != 3){
                    parsed = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                        &v1[0], &v1[1], &v1[2],
                        &v2[0], &v2[1], &v2[2],
                        &v3[0], &v3[1], &v3[2]);
                }
                if(parsed != 9 && parsed != 3){
                    parsed = sscanf(line, "f %d/%d %d/%d %d/%d", 
                        &v1[0], &v1[1],
                        &v2[0], &v2[1],
                        &v3[0], &v3[1]);
                }
                if(parsed != 6 && parsed != 3 && parsed != 9){
                    parsed = sscanf(line, "f %d//%d %d//%d %d//%d", 
                        &v1[0], &v1[2],
                        &v2[0], &v2[2],
                        &v3[0], &v3[2]);
                }
            }
            
            // Only process if we successfully parsed at least 3 vertex indices
            if(v1[0] != 0 && v2[0] != 0 && v3[0] != 0){
                //OBJ indices are 1-based, convert to 0-based
                mesh->indices[iIndex++] = v1[0] - 1;
                mesh->indices[iIndex++] = v2[0] - 1;
                mesh->indices[iIndex++] = v3[0] - 1;

                //attach uvs and normals if present
                if(v1[1] != 0 && temp_uvs != NULL){
                    if(v1[1] - 1 < textureCount) mesh->vertices[v1[0] - 1].uv = temp_uvs[v1[1] - 1];
                    if(v2[1] - 1 < textureCount) mesh->vertices[v2[0] - 1].uv = temp_uvs[v2[1] - 1];
                    if(v3[1] - 1 < textureCount) mesh->vertices[v3[0] - 1].uv = temp_uvs[v3[1] - 1];
                }
                if(v1[2] != 0 && temp_normals != NULL){
                    if(v1[2] - 1 < normalCount) mesh->vertices[v1[0] - 1].normal = temp_normals[v1[2] - 1];
                    if(v2[2] - 1 < normalCount) mesh->vertices[v2[0] - 1].normal = temp_normals[v2[2] - 1];
                    if(v3[2] - 1 < normalCount) mesh->vertices[v3[0] - 1].normal = temp_normals[v3[2] - 1];
                }
                
                // Handle quad - create second triangle
                if(v4[0] != 0){
                    mesh->indices[iIndex++] = v1[0] - 1;
                    mesh->indices[iIndex++] = v3[0] - 1;
                    mesh->indices[iIndex++] = v4[0] - 1;

                    //attach uvs and normals if present
                    if(v4[1] != 0 && temp_uvs != NULL){
                        if(v4[1] - 1 < textureCount) mesh->vertices[v4[0] - 1].uv = temp_uvs[v4[1] - 1];
                    }
                    if(v4[2] != 0 && temp_normals != NULL){
                        if(v4[2] - 1 < normalCount) mesh->vertices[v4[0] - 1].normal = temp_normals[v4[2] - 1];
                    }
                }
            }
        }
    }

    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;
    
    printf("Loaded %d vertices and %d indices\n", vertexCount, indexCount);

    if(temp_uvs) free(temp_uvs);
    if(temp_normals) free(temp_normals);
    
    fclose(f);
    return 0;

}

void freeMesh(Mesh* mesh){
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