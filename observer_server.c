/*
 * The MIT License
 *
 * Copyright 2017 azarias.boutin.
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



#include "observer.h"

#define MAX_CONNECTIONS 10

static int current_proccess_id = 0;

//Current state of the observer : contains the stamps of all the connected processes
static sndmsg_response directory = {.stamp_number = 0, .process =
	{}};

wakeup_response *
wakeup_request_1_svc(void *argp, struct svc_req *rqstp)
{
	static wakeup_response result;
	result.errno = 0;

	if (directory.stamp_number == MAX_CONNECTIONS) {
		result.errno = 1; // Reached maximum connections
		result.process_id = 0;
		return &result;
	}

	result.process_id = current_proccess_id;
	static stamp s;
	s.action_number = 0;
	s.proccess_id = result.process_id;
	directory.process[directory.stamp_number] = s;
	directory.stamp_number++;
	current_proccess_id++;
	printf("Current proccess id = %d\n", current_proccess_id);
	return &result;
}

sndmsg_response *
sndmsg_request_1_svc(void *argp, struct svc_req *rqstp)
{

	return &directory;
}

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
 * @return 
 */
void *
report_action_1_svc(action_report *argp, struct svc_req *rqstp)
{
	static char * result;

	printf("Process number %u is reporting progress : %d - current stamp at %u\n", argp->process_stamp.proccess_id, argp->action_type, argp->process_stamp.action_number);

	return(void *) &result;
}
