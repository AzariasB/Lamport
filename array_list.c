/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "array_list.h"
void list_reserve(list* l,int size);    
list* list_create(){
    list* l = malloc(sizeof(list));
    l->size = 0;
    l->list = malloc(5*sizeof(list));
    l->max_size = 5;
    return l;
}
void list_add(list* l,void* elem){
    if (l->size==l->max_size){
        list_reserve(l,5);
    }
    l->list[l->size]=elem;
    l->size+=1;
}

void list_reserve(list* l,int size){
    void** new_res = malloc((size+l->max_size)*sizeof(void*));
    for (int i =0;i<l->size;i++){
        new_res[i] = l->list[i];
    }
    free(l->list);
    l->list = new_res;
    l->max_size+=size;
}

void* list_at(list* l,int index){
    return l->list[index];
}
void* list_last(list* l){
    if (l->size ==0){
        printf("no last element on empty list\n exiting");
        exit(0);
    }
    return l->list[l->size-1];
}
void list_free(list* l, int (*destructor)(void*)){
    for (int i =0;i<l->size;i++){
        (*destructor)(l->list[i]);
    }
    free(l->list);
    free(l);
}
void* list_rm_at(list* l, int index){
    void* temp = l->list[index];
    l->size-=1;
    for (int i = index;i<l->size;i++){
        l->list[i] = l->list[i+1];
    }
    return temp;
}
int list_find_by_key(list* l, int key, int(*compare)(void*,int)){
    for (int i= 0;i<l->size;i++){
        if ((*compare)(l->list[i],key)){
            return i;
        }
    }
    return -1;
}
