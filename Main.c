#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "lineShader.h"

#define WIDTH 64
#define HEIGHT 32

int main(){
    vec3 vertices[] = {
        0, 0.5, -1,
        -0.5, 0, 1,
        0.5, 0, 1
    };
    //left to right in terms of x
    int indices[] = {
        1, 0, 2
    };


    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    char c;
    //convert 
    for(int i = 0; i < n; i++){
        pixels[i] = toPixel(&vertices[i], WIDTH, HEIGHT);
        terminal[i] = toTerminal(&pixels[i], WIDTH, HEIGHT);
    }
    //test if coords convertion works properly
    for(int i = 0; i < n; i++){
        printf("%2f ", vertices[i].x);
        printf("%2f ", vertices[i].y);
        printf("%2f ", vertices[i].z);
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%2f ", pixels[i].x);
        printf("%2f ", pixels[i].y);
        printf("%2f ", pixels[i].z);
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%2f ", terminal[i].x);
        printf("%2f ", terminal[i].y);
        printf("%2f ", terminal[i].z);
    }
    printf("\n");
    /*
    while(1){
        printf("\e[1;1H\e[2J");
        render(terminal, n);
        for(int i = 0; i < n; i++){
            printf("%2f ", terminal[i].x);
            printf("%2f ", terminal[i].y);
            printf("%2f ", terminal[i].z);
        }
        printf("\n");
    }
    */

    //line testing
    
    int line_len = lineLen(terminal, 1, 0);
    int shape_len = shapeLen(terminal, indices, 3, 0);
    int point_len = pointsLen(terminal, indices, 3, sizeof(indices)/sizeof(int));
    printf("%d\n", line_len);
    printf("%d\n", shape_len);
    printf("%d\n", point_len);

    //render test
    vec3 points[point_len];
    int offset = 0;
    int stride = 3;
    int shape_num = (sizeof(indices)/sizeof(int))/sizeof(stride);
    for(int i = 0; i < shape_num; i++){
        makeShape(terminal, indices, stride, i, points, &offset);
    }
    
    //convert to correct coords
    int final_len = 0;
    for(int i = 0; i < point_len; i++){
        if(points[i].y == points[i+1].x){
            if(points[i].x != points[i+1])
                final_len++;
        }
    }
    int x_off = 0;
    int y_off = 0;
    int y_temp;
    int x_temp;
    for(int i = 0; i < point_len; i += y_off + 1){
        y_temp = y_off;
        y_off = yCheck(points, y_temp);
        for(int j = y_temp; y <= y_off; j += x_off + 1){
            x_temp = x_off;
            x_off = xCheck(points, x_temp);

        }
    }


    return 0;
    
}