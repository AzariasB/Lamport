/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   history_writer.h
 * Author: user
 *
 * Created on 23 mai 2017, 17:48
 */

#ifndef HISTORY_WRITER_H
#define HISTORY_WRITER_H
#include "observer.h"
#include "svg_file.h"
#include "array_list.h"
#include "geometry.h"
#include "saved_action.h"
#include "observer_server.h"
typedef struct action_start action_start;
struct action_start{
    point* start;
    int id;
};
action_start* action_start_create(point* start, int id);
int compare_by_id(void* a, int key);
void compute_svg_background(svg* file, int nb_process);
int get_max_process();

void compute_svg() ;
void write_logs();
#endif /* HISTORY_WRITER_H */