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
/* 
 * File:   observer_server.h
 * Author: azarias
 *
 * Created on 5 mai 2017, 17:54
 */


#include "observer_server.h"

void remove_process_at(int index)
{
	for (int i = index; i < directory.stamp_number - 1; i++) {
		directory.process[i] = directory.process[i + 1];
	}
	directory.stamp_number--;
}

void remove_process(u_int process_id)
{
	for (int i = 0; i < directory.stamp_number; i++) {
		if (directory.process[i].proccess_id == process_id) {
			remove_process_at(i);
			return;
		}
	}
}

wakeup_response * wakeup_request_1_svc(void *argp, struct svc_req *rqstp)
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

sndmsg_response *sndmsg_request_1_svc(void *argp, struct svc_req *rqstp)
{

	return &directory;
}

sndmsg_response *report_action_1_svc(action_report *argp, struct svc_req *rqstp)
{
	printf("Process number %u is reporting progress : %d - current stamp at %u\n", argp->process_stamp.proccess_id, argp->action_type, argp->process_stamp.action_number);
	switch (argp->action_type) {
	case REPORT_SNDMSG:
		break;
	case REPORT_SNDREQ:
		break;
	case REPORT_SNDREP:
		break;
	case REPORT_SNDREL:
		break;
	case REPORT_RCVREQ:
		break;
	case REPORT_RCVREP:
		break;
	case REPORT_RCVREL:
		break;
	case REPORT_RCVMSG:
		break;
	case REPORT_LCLACT:
		break;
	case REPORT_END:
		printf("Process terminated, removing it from list\n");
		remove_process(argp->process_stamp.proccess_id);
		break;
	}

	return &directory;
}
