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
#include <time.h>

static stamp client_stamp;

void local_action(CLIENT *clnt)
{
	printf("Local action\n");
	sleep(1);
}

void send_message(CLIENT *clnt)
{
	char *sndmsg_request_1_arg;
	sndmsg_response *result_2 = sndmsg_request_1((void*) &sndmsg_request_1_arg, clnt);
	if (result_2 == (sndmsg_response *) NULL) {
		clnt_perror(clnt, "call failed");
	} else {
		for (int i = 0; i < result_2->stamp_number; i++) {
			stamp other = result_2->process[i];
			if (other.proccess_id != client_stamp.proccess_id) {
				printf("Need to send another message to : %u\n", other.proccess_id);
				sleep(1);
				break;
			}
		}
	}
}

void request(CLIENT *clnt)
{
	action_report report_action_1_arg;
	report_action_1_arg.action_type = 1;
	void *result_3 = report_action_1(&report_action_1_arg, clnt);
	if (result_3 == (void *) NULL) {
		clnt_perror(clnt, "call failed");
	} else {
		printf("Request going to critical section\n");
		sleep(1);
		//Do something !
	}
}

void
observer_1(char *host)
{

	CLIENT *clnt;



#ifndef DEBUG
	clnt = clnt_create(host, OBSERVER, FIRST_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	char *wakeup_request_1_arg;
	wakeup_response *result_1 = wakeup_request_1((void*) &wakeup_request_1_arg, clnt);
	if (result_1 == (wakeup_response *) NULL || result_1->errno > 0) {
		clnt_perror(clnt, "call failed");
	} else {
		client_stamp.action_number = 0;
		client_stamp.proccess_id = result_1->process_id;
		printf("Process id is : %u\n", client_stamp.proccess_id);
	}
	
	while (1) {
		int action = rand() % 3;
		switch (action) {
		case 0:
			local_action(clnt);
			break;
		case 1:
			send_message(clnt);
			break;
		case 2:
			request(clnt);
			break;
		default:
			break;
		}
	}


#ifndef DEBUG
	clnt_destroy(clnt);
#endif  /* DEBUG */
}

int
main(int argc, char *argv[])
{
	srand(time(NULL));
	char *host;

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	observer_1(host);
	exit(0);
}
