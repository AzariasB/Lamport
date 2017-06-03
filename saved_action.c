/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "saved_action.h"

saved_action* saved_action_create(action_report* action){
    saved_action* s_action = malloc(sizeof(saved_action));
    s_action->action_type = action->action_type;
    s_action->stamp = action->process_stamp.action_number;
    s_action->process = action->process_stamp.proccess_id;
    s_action->action_id = action->process_stamp.action_number;
};

int saved_action_destroy(saved_action* s_action){
    free(s_action);
    return 1;
}

void saved_action_display(saved_action* s_action){
    printf("action at stamp : %d\n type : %d\n sent by %d\n",
        