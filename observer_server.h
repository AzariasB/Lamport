/*
 * The MIT License
 *
 * Copyright 2017 azarias.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   observer_server.h
 * Author: azarias
 *
 * Created on 6 mai 2017, 13:38
 */

#ifndef OBSERVER_SERVER_H
#define OBSERVER_SERVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "observer.h"
#include "saved_action.h"
#include "array_list.h"
#include "svg_file.h"
#include "history_writer.h"
#define MAX_CONNECTIONS 10

//------------
//Variables
//------------


static int current_proccess_id = 0;

//Current state of the observer : contains the stamps of all the connected processes
static sndmsg_response directory = {
    .stamp_number = 0,
    .process =
    {}
};

// History of the actions : list of action types and stamp
list* action_history;
int history_created = 0;
//--------------
//Functions
//--------------

/**
 * Removes the process from the list
 * @param process_id
 */
void remove_process(u_int process_id);

/**
 * Update the process from the given stamp
 * in the directory list
 * @param st
 */
void update_process(stamp st);

/**
 * Called whenever a client
 * boots up, this gives the process a process id
 * 
 * @param argp
 * @param rqstp
 * @return 
 */
wakeup_response * wakeup_request_1_svc(void *argp, struct svc_req *rqstp);

/**
 * 
 * Never used - use "report_action" instead
 * @deprecated
 * @param argp
 * @param rqstp
 * @return 
 */
sndmsg_response *sndmsg_request_1_svc(void *argp, struct svc_req *rqstp);

/**
 * Reports an action from a process,
 * the different actions are :
 *  - 1 = report local action
 *  - 2 = report message received
 *  - 3 = report entering critical section
 *  - 4 = report sending reply
 *  - 5 = report sending release
 *  - 6 = report receiving release
 *  - 
 * @param argp
 * @param rqstp
 * @return the list of process declared to the observer
 */
sndmsg_response *report_action_1_svc(action_report *argp, struct svc_req *rqstp);


void first_call();
#ifdef __cplusplus
}
#endif

#endif /* OBSERVER_SERVER_H */

