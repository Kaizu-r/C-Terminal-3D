#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"
#include "lineShader.h"
#include "utils.h"

#define WIDTH 64
#define HEIGHT 32

int main(){
    vec3 vertices[] = {
        0, 0.5, 0.5,
        -0.25, 0, 0.5,
        0.25, 0, 0.5
    };
    //left to right in terms of x
    int indices[] = {
      1, 0, 2

    };


    int n = sizeof(vertices)/sizeof(vec3);
    vec3 pixels[n];
    vec3 terminal[n];
    char c;
    //transform
    model(vertices, n, 180, 0, 0);
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

    //see if our modified lines are working properly
    vec3 points[point_len];
    
    int offset = 0;
    int stride = 3;
    int shapes = (sizeof(indices)/sizeof(int))/stride;
    for(int i = 0; i < shapes; i++){
        makeShape(terminal, indices, points, stride, i, &offset);
    }

    int final_points_len = zBuffer(points, point_len);
    vec3 final_points[final_points_len];
    //transfer our data
    for(int i = 0; i < final_points_len; i++){
        final_points[i] = points[i];
    }

    //test our new points
    printf("%d\n", final_points_len);
    for(int i = 0; i < final_points_len; i++){
        printf("%.4f ", final_points[i].x);
        printf("%.4f ", final_points[i].y);
        printf("%.4f ", final_points[i].z);
        printf("\n");
    }
    //attempt to render
    render(final_points, final_points_len);

    //render test
    /*
    vec3 points[point_len];
    int offset = 0;
    int stride = 3;
    int shape_num = (sizeof(indices)/sizeof(int))/stride;
    for(int i = 0; i < shape_num; i++){
        makeShape(vertices, indices, points, stride, i, &offset);
    }
    
    //check our points

    //convert to correct coords
    int final_len = 0;
    for(int i = 0; i < point_len; i++){
        if(points[i].y == points[i+1].x){
            if(points[i].x != points[i+1].x) //technically, the z doesnt matter since if they both exist in the same xy space, thats just one pixel regardless of z
                final_len++;
        }
    }

    vec3 final_points[final_len+ 1];
    int x_off = 0;
    int y_off = 0;
    int final_index = 0;
    int y_temp;
    int x_temp;
    while(y_off < point_len){
        y_temp = y_off;
        y_off = yCheck(points, y_temp);
        while(x_off < point_len){
            x_off = y_temp;
            x_temp = x_off;
            x_off = xCheck(points, x_temp);
            final_points[final_index++] = depthTest(points, x_temp, x_off);
        }
    }

    //oh god please work
    /*
    for(int i = 0; i < final_index + 1; i++){
        printf("%.2f ", final_points[i].x);
        printf("%.2f ", final_points[i].y);
        printf("%.2f ", final_points[i].z);
        printf("\n");
    }
    
    
    render(final_points, final_len + 1);
    */

    return 0;
    
}