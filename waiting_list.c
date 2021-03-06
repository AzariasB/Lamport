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

#include "waiting_list.h"

struct waiting_list {
	stamp queue[MAX_PROCESSES];
	int last_pos;
	pthread_mutex_t mutex;
};

int wl_next(waiting_list* wl)
{
	if (wl->last_pos == 0)
		return -1;
	return wl->queue[0].proccess_id;
}

/**
 * Inserts the given stamp
 * at the given position
 * before inserting, will
 * shift all the existing
 * stamps in the array
 * @param wl the waiting list to use
 * @param nw_stamp the stamp to insert
 * @param index the index where to insert the stamp
 */
void insert_at(waiting_list *wl, stamp nw_stamp, int index)
{
	if (index < 0 || index >= MAX_PROCESSES) {
		return;
	}

	//Shift all the existing ones
	for (int i = index; i < MAX_PROCESSES - 1; i++) {
		wl->queue[i + 1] = wl->queue[i];
	}
	wl->queue[index] = nw_stamp;
	wl->last_pos++;
}

/**
 * Checks wether the stamp s1 is lower than
 * the stamp s2
 * @param s1
 * @param s2
 * @return 
 */
int is_lower_than(stamp s1, stamp s2)
{
	return s1.action_number < s2.action_number || (s1.action_number == s2.action_number && s1.proccess_id < s2.proccess_id);
}

waiting_list* wl_create()
{
	waiting_list *wl = malloc(sizeof(waiting_list));
	wl->last_pos = 0;
	pthread_mutex_init(&wl->mutex, NULL);
	return wl;
}

void wl_destroy(waiting_list* wl)
{
	pthread_mutex_destroy(&wl->mutex);
	free(wl);
	wl = 0;
}

void wl_push(waiting_list* wl, stamp nw_stamp)
{
	pthread_mutex_lock(&wl->mutex);
	if (wl->last_pos == MAX_PROCESSES) {
		pthread_mutex_unlock(&wl->mutex);
		return; //not possible to insert
	}

	for (int i = 0; i < wl->last_pos; i++) {
		stamp s = wl->queue[i];
		if (is_lower_than(nw_stamp, s)) {
			insert_at(wl, nw_stamp, i);
			pthread_mutex_unlock(&wl->mutex);
			return;
		}
	}
	//Not inserted : insert at the end
	wl->queue[wl->last_pos] = nw_stamp;
	wl->last_pos++;
	pthread_mutex_unlock(&wl->mutex);
}

int wl_isnext(waiting_list* wl, stamp st)
{
	pthread_mutex_lock(&wl->mutex);
	int res = (wl->last_pos > 0 && wl->queue[0].proccess_id == st.proccess_id);
	pthread_mutex_unlock(&wl->mutex);
	return res;
}

void wl_shift(waiting_list *wl, stamp s)
{
	pthread_mutex_lock(&wl->mutex);
	//1 - find the index
	int index;
	for (index = 0; index < MAX_PROCESSES; index++) {
		if (wl->queue[index].proccess_id == s.proccess_id)
			break;
	}
	if (index == MAX_PROCESSES) {
		pthread_mutex_unlock(&wl->mutex);
		return;
	}

	//2 - remove it
	for (; index < wl->last_pos; index++) {
		wl->queue[index] = wl->queue[index + 1];
	}
	if (wl->last_pos > 0)
		wl->last_pos--;
	pthread_mutex_unlock(&wl->mutex);
}

void wl_print(waiting_list* wl)
{
	pthread_mutex_lock(&wl->mutex);
	printf("[");
	for (int i = 0; i < wl->last_pos; i++) {
		printf("{counter: %u, id: %u}", wl->queue[i].action_number, wl->queue[i].proccess_id);
	}
	printf("]\n");
	pthread_mutex_unlock(&wl->mutex);
}