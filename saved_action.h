/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   saved_action.h
 * Author: user
 *
 * Created on 21 mai 2017, 18:00
 */

#ifndef SAVED_ACTION_H
#define SAVED_ACTION_H
#include "observer.h"
typedef struct saved_action saved_action; 
struct saved_action {
    int action_type;
    int process;
    int stamp;
    int action_id;
};

saved_action* saved_action_create(action_report* action);

int saved_action_destroy(saved_action* s_action);

void saved_action_display(saved_action* s_action);
#endif /* SAVED_ACTION_H */

