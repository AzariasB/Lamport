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
 * File:   observer_client.c
 * Author: azarias
 *
 * Created on 5 mai 2017, 17:54
 */


#include "observer_client.h"

void dequeue_requests()
{
	for (int i = 1; i < MAX_REQUESTS; i++) {
		requests[i - 1] = requests[i];
	}

	if (reqs_pos > 0)
		reqs_pos--;

}

void enqueue_request(client_request nw_req)
{
	pthread_mutex_lock(&m_requests);
	if (reqs_pos < MAX_REQUESTS) {
		requests[reqs_pos] = nw_req;
		reqs_pos++;
	}
	pthread_mutex_unlock(&m_requests);
}

void report(int action)
{
	action_report report_action_1_arg;
	report_action_1_arg.action_type = action;
	report_action_1_arg.process_stamp = client_stamp;
	sndmsg_response *result_3 = report_action_1(&report_action_1_arg, client);
	if (result_3 == (void *) NULL) {
		clnt_perror(client, "call failed");
	}
}

void handle_request(client_request req)
{
	switch (req.request_id) {
	case REQUEST_REQUEST:
		printf("Received a request, adding stamp to list\n");
		report(REPORT_RCVREQ);
		wl_push(wl, req.sender); //add to the waiting queue
		socket_client(req.sender.proccess_id, REQUEST_REPLY); //answer to the sender
		report(REPORT_SNDREP);
		break;
	case REQUEST_RELEASE:
		printf("Received a release, removing last stamp from list\n");
		report(REPORT_RCVREL);
		//remove the next process
		wl_shift(wl);
		//Check if can enter in critical section
		check_cs();
		break;
	case REQUEST_REPLY:
		report(REPORT_RCVREP);
		//Decrease the counter
		reply_counter--;
		printf("Received a reply, decremented counter to %d\n", reply_counter);
		//Check if can enter in critical section
		check_cs();
		break;
	case REQUEST_MESSAGE:
		report(REPORT_RCVMSG);
		printf("Received message from %u\n", req.sender.proccess_id);
		break;
	default:
		break;
	}

}

void *socket_server()
{
	printf("Socket server, waiting for any requests ...\n");
	int sockfd, nwsockfd;
	struct sockaddr_un cli_addr;
	struct sockaddr_un srv_addr;

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("Failed to create socket server\n");
		pthread_exit(NULL);
	}
	bzero((char*) &srv_addr, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	sprintf(srv_addr.sun_path, "/tmp/localSocket.%u", client_stamp.proccess_id);
	socklen_t srvlen = strlen(srv_addr.sun_path) + sizeof(srv_addr.sun_family);

	if (bind(sockfd, (struct sockaddr*) &srv_addr, srvlen) < 0) {
		printf("Failed to bind server socket\n");
		pthread_exit(NULL);
	}

	listen(sockfd, 5);


	while (1) {
		socklen_t clilen = sizeof(cli_addr);
		printf("Waiting for a new connection ...\n");
		nwsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

		if (nwsockfd < 0) {
			printf("Error while accepting socket");
		}

		static client_request req;
		read(nwsockfd, &req, sizeof(client_request));
		//		printf("Server received request %d from %u\n", req.request_id, req.sender.proccess_id);
		handle_request(req);
	}

	pthread_exit(NULL);
}

void local_action()
{
	printf("Local action\n");
	sleep(1);
}

void socket_client(u_int contact_id, int request_id)
{
	int sockf, srvlen;
	struct sockaddr_un srv_addr;

	bzero((char*) &srv_addr, sizeof(srv_addr));

	srv_addr.sun_family = AF_UNIX;
	sprintf(srv_addr.sun_path, "/tmp/localSocket.%u", contact_id);
	srvlen = strlen(srv_addr.sun_path) + sizeof(srv_addr.sun_family);
	if ((sockf = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Error while creating the socket\n");
		return;
	}

	if (connect(sockf, (struct sockaddr*) &srv_addr, srvlen) < 0) {
		perror("Error while connection to server\n");
		return;
	}
	client_request req;
	req.request_id = request_id;
	req.sender = client_stamp;

	write(sockf, &req, sizeof(client_request));
	printf("Contacted client %u\n", contact_id);
}

void check_cs()
{
	if (reply_counter == 0 && wl_isnext(wl, client_stamp)) {
		sem_post(&sem_cs);
	}
}

void send_message()
{
	char *sndmsg_request_1_arg;
	sndmsg_response *result_2 = sndmsg_request_1((void*) &sndmsg_request_1_arg, client);
	if (result_2 == (sndmsg_response *) NULL) {
		clnt_perror(client, "call failed");
	} else {
		for (int i = 0; i < result_2->stamp_number; i++) {
			stamp other = result_2->process[i];
			if (other.proccess_id != client_stamp.proccess_id) {
				socket_client(other.proccess_id, REQUEST_MESSAGE);
				return;
			}
		}
		//If still here : no other process was found
		printf("Could not send message, other process not found\n");
	}
	sleep(2);
}

void request()
{
	action_report report_action_1_arg;
	report_action_1_arg.action_type = REPORT_SNDREQ;
	report_action_1_arg.process_stamp = client_stamp;
	sndmsg_response *result_3 = report_action_1(&report_action_1_arg, client);
	if (result_3 == (void *) NULL) {
		clnt_perror(client, "call failed");
	} else {
		printf("Request going to critical section need to ask to %u other processes\n", result_3->stamp_number - 1);
		reply_counter = result_3->stamp_number - 1;
		wl_push(wl, client_stamp);
		//Send a request to all other processes
		for (int i = 0; i < result_3->stamp_number; i++) {
			stamp s = result_3->process[i];
			if (s.proccess_id != client_stamp.proccess_id) {
				socket_client(s.proccess_id, REQUEST_REQUEST);
			}
		}
		//Waiting for the reply + being next in the queue
		//Don't need to wait if no other processes
		if (reply_counter > 0) {
			sem_wait(&sem_cs);
		}
		printf("Entering critical section...");
		fflush(stdout);
		sleep(2);
		printf("exiting critical section !\n");
		wl_shift(wl);
		report(REPORT_SNDREL);
		for (int i = 0; i < result_3->stamp_number; i++) {
			stamp s = result_3->process[i];
			if (s.proccess_id != client_stamp.proccess_id) {
				socket_client(s.proccess_id, REQUEST_RELEASE);
				//Warn all the other processes
			}
		}
	}
}

void main_loop()
{
	while (1) {
		int action = rand() % 3;
		switch (action) {
		case 0:
			local_action(client);
			report(REPORT_LCLACT);
			break;
		case 1:
			send_message(client);
			report(REPORT_SNDMSG);
			break;
		case 2:
			request(client);
			break;
		default:
			break;
		}
		client_stamp.action_number++;
	}
}

void observer_1(char *host)
{
	pthread_t server;


#ifndef DEBUG
	client = clnt_create(host, OBSERVER, FIRST_VERSION, "udp");
	if (client == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	char *wakeup_request_1_arg;
	wakeup_response *result_1 = wakeup_request_1((void*) &wakeup_request_1_arg, client);
	if (result_1 == (wakeup_response *) NULL || result_1->errno > 0) {
		clnt_perror(client, "call failed");
	} else {
		client_stamp.action_number = 0;
		client_stamp.proccess_id = result_1->process_id;
		printf("Process id is : %u\n", client_stamp.proccess_id);
	}
	pthread_create(&server, NULL, socket_server, NULL);
	main_loop();

	pthread_join(server, NULL);
#ifndef DEBUG
	clnt_destroy(client);
#endif  /* DEBUG */
}

/**
 * TODO: listen for SIGTERM and others
 * to detect when killed and send a signal to
 * observer to declared the death of the process
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char *argv[])
{
	srand(time(NULL));
	wl = wl_create();
	bzero(requests, sizeof(requests)); //reset all cells of "requests"

	sem_init(&sem_cs, 0, 0); //only shared between thread, starting with value '0'
	pthread_mutex_init(&m_requests, NULL);
	char *host;

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	observer_1(host);

	pthread_mutex_destroy(&m_requests);
	sem_destroy(&sem_cs);
	wl_destroy(wl);
	exit(0);
}
