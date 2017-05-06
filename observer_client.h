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
 * File:   observer_client.h
 * Author: azarias
 *
 * Created on 5 mai 2017, 21:51
 */

#ifndef OBSERVER_CLIENT_H
#define OBSERVER_CLIENT_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "observer.h"
#include "waiting_list.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <bits/string3.h>
#include <bits/stdio2.h>

#define REQUEST_MESSAGE 1
#define REQUEST_REQUEST 2
#define REQUEST_REPLY   3
#define REQUEST_RELEASE 4

#define MAX_REQUESTS 64

//Variables

typedef struct
{
    stamp sender;
    int request_id;
} client_request;

stamp client_stamp;

waiting_list *wl;

client_request requests[MAX_REQUESTS];

int reqs_pos = 0;

CLIENT *client;

int reply_counter = 0;

/**
 * Mutex for the requests array
 */
pthread_mutex_t m_requests;


//Functions



/**
 * Remove the first request in the list
 * and shift all the remaining ones
 */
void dequeue_requests();

/**
 * Add the given request at the end
 * of the requests queue
 * 
 * @param nw_req
 */
void enqueue_request(client_request nw_req);

/**
 * Handles the requests in the list
 * in order.
 * For each request, will perform the
 * necessray task
 * At the end, resets the request counter to 0
 */
void handle_request(client_request req);

/**
 * Waits for any incoming connection
 * to send a request/reply/release
 * 
 */
void *socket_server();


/**
 * Performs a local action
 * (sleeps)
 */
void local_action();

/**
 * Connects a socket to the process with the given id
 * and sends the request with the given id
 * 
 * @param contact_id the process's id to contact
 * @param request_id the request id to send
 */
void socket_client(u_int contact_id, int request_id);

/**
 * Send message to another process means :
 *  - Telling the server we want he list of other processes
 *  - Choosing a random process in the list, other than us)
 *  - Opening a client socket with this random process
 *  - Sending a message
 *  - Closing the socket
 * 
 * As simple as that !
 * 
 * @param clnt
 */
void send_message();

/**
 * A request is made before entering in critcal section
 * the critcal section will be, here implemented as a simple 'sleep'
 * in a real example, it would actually lock some ressources and 
 * use a critical resource.
 * To do so, we need to :
 *  - Put the 'request' in its own waiting queu
 *  - Tell the observer we are going in critical section, and we need the the list of other processes
 *  - ask to the other processes (send them a 'request' message with sockets)
 *  - wait the reception of all the reply
 *  - entering in critical section
 *  - Send a "release" message to the others
 * 
 * @param clnt
 */
void request();

/**
 * Main loop of the program does the following : 
 * 1 - perorms a random taks
 * 2 - Increments the action counter
 * 3 - processes the received requests
 */
void main_loop();

/**
 * Entry point of the function :
 * 
 * Will first tell the observer it exists
 * Then, will loop forever, choosing between three action choices : 
 *  - 1 : do a local action (sleep) 
 *  - 2 : send a "message" to another process
 *  - 3 : ask and wait to enter in critical section
 * 
 * These three things will all increment the stamp value by one
 * 
 * @param host
 */
void observer_1(char *host);

#ifdef __cplusplus
}
#endif

#endif /* OBSERVER_CLIENT_H */
