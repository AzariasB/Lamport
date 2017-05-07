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
 * File:   waiting_list.h
 * Author: azarias
 *
 * Created on 6 mai 2017, 10:32
 */


#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include "observer.h"
#include <pthread.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * The waiting list is a thread
 * safe queue of stamps
 * Every call to the 'public' functions
 * restult in a mutex lock until completion
 */
typedef struct waiting_list waiting_list;

/**
 * Creates a new waiting list
 * @return the newly craeted waiting list
 */
waiting_list *wl_create();

/**
 * Destroys the given waiting list
 * @param wl
 */
void wl_destroy(waiting_list* wl);

/**
 * Add a new stamp in the waiting list
 * insert it in a sorted way
 * @param nw_stamp
 */
void wl_push(waiting_list *wl, stamp nw_stamp);

/**
 * checks if the given stamp
 * is first in the given waiting_list
 * @param wl the waiting list to use
 * @param st the element to check
 * @return 1 if the element is first in the waiting list, 0 otherwise
 */
int wl_isnext(waiting_list *wl, stamp st);

/**
 * Removes the first element ofthe list
 * and shifts all the others
 * @param wl the waiting list to use
 * @param s the stamp of the process to remove
 */
void wl_shift(waiting_list *wl, stamp s);

/**
 * Returns the id of the next element in the list
 * @param wl the list to use
 * @return the id of the next process
 */
int wl_next(waiting_list *wl);


/**
 * Prints out the given waiting list
 * @param wl the waiting list
 */
void wl_print(waiting_list *wl);

#ifdef __cplusplus
}
#endif

#endif /* WAITING_LIST_H */

