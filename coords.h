#ifndef COORDS_H
#define COORDS_H
#include <math.h>
#include <stdlib.h>
#include "vertex.h"

//set up for global coords
/*
    given width x and heigth y, the center must be (0,0) and corners must be(x/2, y/2)
    (-x/2, y/2)             (x/2, y/2)


                   (0,0)      


    (-x/2, -y/2)            (x/2, -y/2) 
    normalize global to (-1 -> 1, -1 -> 1)
    to get "pixel" coords based on normalized coords, multiply by 1/2 of x and y respectively
*/
//setup for print coords
/*
    for x in terminal "coords", print space, print next line for y
    since technically, terminal "coords" are
    (0,0)               (x, 0)


             (x/2/y2)


    (0, y)              (x,y)
    we have to convert pixel coords to terminal "coords"
    terminal.x = pixel.x + width/2 + shift value
    terminal.y = -(pixel/y - height/2) 
*/

//scales normalized vertex coords to "pixel" coords
vec3 toPixel(vec3* vertex, int width, int height){
    vec3 pixel;
    //truncate
    pixel.x = (int) (vertex->x * width/2);
    pixel.y = (int) (vertex->y * height/2);
    pixel.z = vertex->z;
    return pixel;
}

//turns pixel coords to terminal coords
vec3 toTerminal(vec3 pixel, int width, int height){
    vec3 terminal; 
    terminal.x = (pixel.x + 1);
    terminal.x *= 0.5 * width;
    terminal.y =  (pixel.y + 1);
    terminal.y *= 0.5 * height;
    terminal.z = (int) pixel.z;
    terminal.l = pixel.l;

    return terminal;
}

vec3 revert(vec3 point, int WIDTH, int HEIGHT){
    vec3 new_p;
    new_p.x = point.x - WIDTH/2;
    new_p.y = -(point.x - HEIGHT/2);
    new_p.z = point.z;
    new_p.l = point.l;

    return new_p;

}

#endif