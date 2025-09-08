#ifndef LIST_H
#define LIST_H

#include "utils.h"
#include "vertex.h"


typedef struct list_node{
    vec2 v;
    struct list_node * next;
    struct list_node* prev;

}List_node;

typedef struct List{
    List_node* front;
    List_node* back;
    int size;
}List;

List_node* createListNode(vec2 v){
    List_node* m = (List_node*) malloc (sizeof(List_node));
    m->v = v;
    m->next = NULL;
    m->prev = NULL;

    return m;
}

List* createList(){
    List* m = (List*) malloc(sizeof(List));
    m->back = NULL;
    m->front = NULL;
    m->size = 0;

    return m;
}

void deleteListNode(List_node** m){
    free(*m);
}
void deleteList(List** m){
    if((*m)->front == NULL)
        return;
    
    while((*m)->front != NULL){
        List_node* p = (*m)->front;
        (*m)->front = (*m)->front->next;
        free(p);
    }

    free((*m)->back);
    free(*m);

}

void pushFront(List **m, vec2 v){
    List_node* n = createListNode(v);
    (*m)->size++;

    if((*m)->front == NULL){
        (*m)->front = n;
        (*m)->back = n;
        return;
    }

    n->next = (*m)->front;
    n->next->prev = n;
    (*m)->front = n;


}

void pushBack(List **m, vec2 v){
    List_node* n = createListNode(v);
    (*m)->size++;

    if((*m)->back == NULL){
        (*m)->front = n;
        (*m)->back = n;
        return;
    }

    n->prev = (*m)->back;
    n->prev->next = n;
    (*m)->back = n;
}


void popFront(List **m){
    if((*m)->front == NULL)
        return;
    
    List_node* p = (*m)->front;
    (*m)->front = (*m)->front->next;
    (*m)->front->prev = NULL;
    (*m)->size--;

    free(p);
}

void popBack(List **m){
    if((*m)->back == NULL)
        return;

    List_node *p = (*m)->back;
    (*m)->back = (*m)->back->prev;
    (*m)->back->next = NULL;
    (*m)->size--;

    free(p);
}


#endif