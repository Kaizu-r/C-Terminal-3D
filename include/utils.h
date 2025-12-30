#ifndef M_PI
#define M_PI 3.14159265
#endif
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "vertex.h"
#include <math.h>
#include <time.h>

float rad(int deg);
void clear();
void wait(int waiting);
mat3 rotateX(float radians);
mat3 rotateY(float radians);
mat3 rotateZ(float radians);
mat3 matMultiply(mat3 mat1, mat3 mat2);
mat3 matTransform(mat3 matx, mat3 maty, mat3 matz);
float dot(vec3 vec1, vec3 vec2);
void model(vec3 vert[], int size, vec3 deg);
void proj(vec3 *vert, int far, int near, int fov, int WIDTH, int HEIGHT);
float precision(float num, float factor);
void translation(vec3 vert[], int size, vec3 trans);
void scale(vec3 vert[], int size, float factor);
void scale(vec3 vert[], int size, float factor);
void view(vec3 vert[], int size, vec3 trans, vec3 rot);
void camera(vec3 vert[], int size, float focal);
void modelTransform(vec3 vert[], int size, float factor, vec3 deg, vec3 trans);
void merge3v(vec3 vertices[], int i1, int j1, int i2, int j2);
void mergesort3v(vec3 vertices[], int i, int j);
void merge2v(vec2 vertices[], int i1, int j1, int i2, int j2);
void mergesort2v(vec2 vertices[], int i, int j);
float fast_inRoot(float number);
float vect_distance(vec3 vec1, vec3 vec2);
vec3 normal(vec3 vert1, vec3 vert2, vec3 vert3);
vec3 v_angle(vec3 vert1, vec3 vert2);
tri triangleBuild(vec3 vert[], int index[], int offset);
void swap(vec3 *p1, vec3 *p2);
void interpolate(float res[], int n, float x0, float y0, float x1, float y1);
vec3 normalize(vec3 v);


//degrees to radians convertion
float rad(int deg){
    float y = deg * M_PI/180;
    return y;
}

void clear(){
    printf("\e[1;1H\e[1J\e[2J\e[3J");
}

void wait(int waiting){
    int ms = (CLOCKS_PER_SEC/waiting);
    clock_t start_time = clock();
    clock_t curr;
    int elapsed;
    do
    {
        curr = clock();
        elapsed = (curr - start_time);
    } while (elapsed < ms);
    
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
    mat3 transform = matMultiply(matx, maty);
    transform = matMultiply(transform, matz);
    return transform;
}

float dot(vec3 vec1, vec3 vec2){

    float prod = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
    return prod;
}

//responsible for rotating our vertices based on model transfomration
void model(vec3 vert[], int size, vec3 deg){
    float radX = rad(deg.x);
    float radY = rad(deg.y);
    float radZ = rad(deg.z);


    //get our transformation matrix
    

    mat3 transform = matTransform(rotateX(radX), rotateY(radY), rotateZ(radZ));

    //transform each vertex
    int vertNum = size;
    
    vec3 temp;
    
    for(int i = 0; i < vertNum; i++){
        //store first to temp
        temp.x = vert[i].x * transform.matrix[0][0] + vert[i].y * transform.matrix[0][1] + vert[i].z * transform.matrix[0][2];
        temp.y = vert[i].x * transform.matrix[1][0] + vert[i].y * transform.matrix[1][1] + vert[i].z * transform.matrix[1][2];
        temp.z = vert[i].x * transform.matrix[2][0] + vert[i].y * transform.matrix[2][1] + vert[i].z * transform.matrix[2][2];

        vert[i] = temp;
    }
}

//creates the projected coordinates  
void proj(vec3 *vert, int farPlane, int nearPlane, int fov, int WIDTH, int HEIGHT){

    float s = 1.0f / tanf(fov * 0.5f * M_PI / 180.0f);
    float aspect = (float)WIDTH / (float)HEIGHT;
        
    // Standard perspective projection matrix (camera faces -Z)
    // Maps z = -near -> z_ndc = -1, z = -far -> z_ndc = 1
    float A = -(farPlane + nearPlane) / (farPlane - nearPlane);
    float B = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    
    float z_view = vert->z;  // Save original view-space Z
    float z_clip = A * z_view + B;
    float x_clip = vert->x * (s / aspect);
    float y_clip = vert->y * s;
    float w_clip = -z_view;

    if(w_clip != 0.0f){
        vert->x = x_clip / w_clip;
        vert->y = y_clip / w_clip;
        vert->z = z_clip / w_clip;
    }

       
}

float precision(float num, float factor){
    return ((int) num * factor)/factor;
}

//translates the vertex coords. Essentially, we move the world
void translation(vec3 vert[], int size, vec3 trans){
    for(int i = 0; i < size; i++){
        vert[i].x += trans.x;
        vert[i].y += trans.y;
        vert[i].z += trans.z;
    }
}

void scale(vec3 vert[], int size, float factor){
    for(int i = 0; i < size; i++){
        vert[i].x *= factor;
        vert[i].y *= factor;
        vert[i].z *= factor;
    }
}

//setup the view matrix
void view(vec3 vert[], int size, vec3 trans, vec3 rot){
    // apply inverse camera rotation then translate into camera space
    // p_view = R^T * (p_world - t_cam)
    float radX = rot.x * (M_PI/180.0f);
    float radY = rot.y * (M_PI/180.0f);
    float radZ = rot.z * (M_PI/180.0f);

    // inverse rotation: negate angles
    mat3 transform = matTransform(rotateX(-radX), rotateY(-radY), rotateZ(-radZ));

    for(int i = 0; i < size; i++){
        vec3 temp;
        // subtract camera translation
        temp.x = vert[i].x - trans.x;
        temp.y = vert[i].y - trans.y;
        temp.z = vert[i].z - trans.z;

        // apply inverse rotation
        vec3 out;
        out.x = temp.x * transform.matrix[0][0] + temp.y * transform.matrix[0][1] + temp.z * transform.matrix[0][2];
        out.y = temp.x * transform.matrix[1][0] + temp.y * transform.matrix[1][1] + temp.z * transform.matrix[1][2];
        out.z = temp.x * transform.matrix[2][0] + temp.y * transform.matrix[2][1] + temp.z * transform.matrix[2][2];

        vert[i] = out;
    }
}

//setup the camera
void camera(vec3 vert[], int size, float focal){
    for(int i = 0; i < size; i++){
        vert[i].x *= focal;
        vert[i].y *= focal;
    }
}

void modelTransform(vec3 vert[], int size, float factor, vec3 deg, vec3 trans){
    scale(vert, size, factor);
    model(vert, size, deg);
    translation(vert, size, trans);
}

//custom merge sort algo
void merge3v(vec3 vertices[], int i1, int j1, int i2, int j2){
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
        else if(vertices[start1].z < vertices[start2].z) //now we check for z if x and y are equal
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

void mergesort3v(vec3 vertices[], int i, int j){
    if(i >= j)
        return;
    int mid = (i + j)/2;
    mergesort3v(vertices, i, mid);
    mergesort3v(vertices, mid+1, j);
    merge3v(vertices, i, mid, mid+1, j);
}

//merge sort for v2
void merge2v(vec2 vertices[], int i1, int j1, int i2, int j2){
    int start1, start2, k;
    vec2 temp[j2 + 1];
    
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

void mergesort2v(vec2 vertices[], int i, int j){
    if(i >= j)
        return;
    int mid = (i + j)/2;
    mergesort2v(vertices, i, mid);
    mergesort2v(vertices, mid+1, j);
    merge2v(vertices, i, mid, mid+1, j);
}

float fast_inRoot(float number){
    long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}



float vect_distance(vec3 vec1, vec3 vec2){
    vec3 diff = {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z}; 
    return sqrt(dot(diff, diff));
}

//calcs normal vector
vec3 normal(vec3 vert1, vec3 vert2, vec3 vert3){
    
    vec3 A = {vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z};
    vec3 B = {vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z};

    vec3 norm;
    norm.x = A.y * B.z - A.z * B.y;
    norm.y = A.z * B.x - A.x * B.z;
    norm.z = A.x * B.y - A.y * B.x;



    float inverse_root = fast_inRoot(norm.x*norm.x + norm.y*norm.y + norm.z * norm.z);
    
    norm.x *= inverse_root;
    norm.y *= inverse_root;
    norm.z *= inverse_root;

    return norm;
}

//angle between two vectors
vec3 v_angle(vec3 vert1, vec3 vert2){
    vec3 dir;
    dir.x = vert2.x -vert1.x;
    dir.y = vert2.y - vert1.y;
    dir.z = vert2.z - vert1.z;

    //printf("%f %f %f ", dir.x, dir.y, dir.z);
    float inverse_norm = fast_inRoot(dot(dir, dir));
    dir.x = dir.x * inverse_norm;
    dir.y = dir.y * inverse_norm;
    dir.z = dir.z * inverse_norm;

    return dir;
}


tri triangleBuild(vec3 vert[], int index[], int offset){
    tri tri1;
    tri1.v1 = vert[index[offset*3]];
    tri1.v2 = vert[index[offset*3 + 1]];
    tri1.v3 = vert[index[offset*3 + 2]];

    return tri1;
}

void swap(vec3 *p1, vec3 *p2){
    vec3 temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}


//x independent, y dependent
void interpolate(float res[], int n, float x0, float y0, float x1, float y1){
    if(n == 0){
        return;
    }

    float slope = (y1 - y0)/(float) n;
    for(int i = 0; i < n; i++){
        res[i] = y0 + (i*slope);
    }
}

vec3 normalize(vec3 v){
    float invlen = fast_inRoot(v.x*v.x + v.y*v.y + v.z*v.z);
    vec3 newv = {v.x * invlen, v.y * invlen, v.z * invlen};
    return newv;
}



















#endif