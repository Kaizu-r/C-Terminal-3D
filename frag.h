#ifndef FRAG_H
#define FRAG_H
#include "vertex.h"
#include "utils.h"
//implement our fragment renderer which is hella difficult btw
/*
    given a set of points of a shape
    sort the points using our merge sort algorithm (should be done in make shape)
    simply iterate for every y value
     total_y =   points[line_offset] - points[start_index] + 1
     given j = points_offset and k = j
    for every x in y, we simply check if the current and next element has the same y, if they do, then
    we check if their difference is 1 or less. if it is, we move our current to the next element
    if not, then k = next element
    then simply draw a line from curren to k
    for(int i = 0; i < total_y; i++){
        
    }
*/
//rasterizer per line



#endif