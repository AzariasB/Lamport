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

void handle_requests()
{
	printf("Need to handle %d request\n", reqs_pos);
	if (reqs_pos > 0) {
		pthread_mutex_lock(&m_requests);
		for (int i = 0; i < reqs_pos; i++) {
			client_request req = requests[i];
			switch (req.request_id) {
			case REQUEST_RELEASE:
				dequeue_requests();
				break;
			case REQUEST_REPLY:
				//Remove necessary reply from list
				break;

			case REQUEST_REQUEST:
				/**
				 * When receiving a request : 
				 * 
				 */
				break;
			case REQUEST_MESSAGE:
			default:
				break;
			}
		}
		reqs_pos = 0;
		pthread_mutex_unlock(&m_requests);
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
		printf("Server received request %d from %u\n", req.request_id, req.sender.proccess_id);
		enqueue_request(req);
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

	//Nothing to receive from server ...
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
}

void request()
{
	action_report report_action_1_arg;
	report_action_1_arg.action_type = 1;
	report_action_1_arg.process_stamp = client_stamp;
	void *result_3 = report_action_1(&report_action_1_arg, client);
	if (result_3 == (void *) NULL) {
		clnt_perror(client, "call failed");
	} else {
		printf("Request going to critical section\n");
		sleep(1);
		//Do something !
	}
}

void main_loop()
{
	while (1) {
		int action = rand() % 3;
		switch (action) {
		case 0:
			local_action(client);
			break;
		case 1:
			send_message(client);
			break;
		case 2:
			request(client);
			break;
		default:
			break;
		}
		client_stamp.action_number++;
		handle_requests();
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

	bzero(requests, sizeof(requests)); //reset all cells of "requests"

	pthread_mutex_init(&m_requests, NULL);
	char *host;

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	observer_1(host);

	pthread_mutex_destroy(&m_requests);
	exit(0);
}
