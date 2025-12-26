#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "utils.h"
#include "vertex.h"

typedef struct list_node{
    void* v;
    struct list_node * next;
    struct list_node* prev;
} List_node;

typedef struct List{
    List_node* front;
    List_node* back;
    int size;
} List;

List_node* createListNode(void* v);
List* createList();
void deleteListNode(List_node** m);
void deleteList(List** m);
void pushFront(List **m, void* v);
void pushBack(List **m, void* v);
void popFront(List **m);
void popBack(List **m);

List_node* createListNode(void* v){
    List_node* m = (List_node*) malloc (sizeof(List_node));
    if (!m) return NULL;
    m->v = v;
    m->next = NULL;
    m->prev = NULL;
    return m;
}

List* createList(){
    List* m = (List*) malloc(sizeof(List));
    if (!m) return NULL;
    m->back = NULL;
    m->front = NULL;
    m->size = 0;
    return m;
}

/* Frees node->v (if non-NULL) and the node itself. Sets *m to NULL. */
void deleteListNode(List_node** m){
    if (m == NULL || *m == NULL) return;
    if ((*m)->v) free((*m)->v);
    free(*m);
    *m = NULL;
}

/* Frees all nodes and then the List struct. Sets *m to NULL. */
void deleteList(List** m){
    if (m == NULL || *m == NULL) return;
    List_node* cur = (*m)->front;
    while(cur != NULL){
        List_node* next = cur->next;
        deleteListNode(&cur);
        cur = next;
    }
    free(*m);
    *m = NULL;
}

void pushFront(List **m, void* v){
    if (m == NULL || *m == NULL) return;
    List_node* n = createListNode(v);
    if (!n) return;
    (*m)->size++;
    if((*m)->front == NULL){
        (*m)->front = n;
        (*m)->back = n;
        return;
    }
    n->next = (*m)->front;
    (*m)->front->prev = n;
    (*m)->front = n;
}

void pushBack(List **m, void* v){
    if (m == NULL || *m == NULL) return;
    List_node* n = createListNode(v);
    if (!n) return;
    (*m)->size++;
    if((*m)->back == NULL){
        (*m)->front = n;
        (*m)->back = n;
        return;
    }
    n->prev = (*m)->back;
    (*m)->back->next = n;
    (*m)->back = n;
}

void popFront(List **m){
    if (m == NULL || *m == NULL) return;
    if((*m)->front == NULL) return;
    List_node* p = (*m)->front;
    if ((*m)->front == (*m)->back) { /* single element */
        (*m)->front = NULL;
        (*m)->back = NULL;
    } else {
        (*m)->front = (*m)->front->next;
        if ((*m)->front) (*m)->front->prev = NULL;
    }
    (*m)->size--;
    deleteListNode(&p);
}

void popBack(List **m){
    if (m == NULL || *m == NULL) return;
    if((*m)->back == NULL) return;
    List_node *p = (*m)->back;
    if ((*m)->front == (*m)->back) { /* single element */
        (*m)->front = NULL;
        (*m)->back = NULL;
    } else {
        (*m)->back = (*m)->back->prev;
        if ((*m)->back) (*m)->back->next = NULL;
    }
    (*m)->size--;
    deleteListNode(&p);
}

#endif