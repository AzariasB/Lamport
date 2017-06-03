/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   array_list.h
 * Author: user
 *
 * Created on 21 mai 2017, 18:35
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
typedef struct array_list list;

struct array_list {
    void** list;
    int size;
    int max_size;
};

void list_reserve(list* l,int size);    
list* list_create();
void list_add(list* l,void* elem);
void* list_at(list* l,int index);
void* list_last(list* l);
void list_free(list* l, int (*destructor)(void*));
void* list_rm_at(list* l, int index);
int list_find_by_key(list* l, int key, int(*compare)(void*,int));
#endif /* ARRAY_LIST_H */

