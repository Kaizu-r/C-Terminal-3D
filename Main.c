#include <stdio.h>
#include <stdlib.h>

#include "vertShader.h"
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
    int m = abs(terminal[1].x - terminal[0].x) + 1;
    int o = abs(terminal[1].y - terminal[0].y) + 1;
    //ternary operator for testing purposes
    int p = (m < o) ? o : m;
    vec3 model[p];
    lineDraw(&terminal[1], &terminal[0], model, m, o);

    //try to render a line
    render(model, p);
    for(int i = 0; i < p; i++){
        printf("%2f ", model[i].x);
        printf("%2f ", model[i].y);
        printf("%2f ", model[i].z);
        printf("\n");
    }
    return 0;
    
}