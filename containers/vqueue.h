#pragma once

/*
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
**
** Lock free queue.
** https://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/

#include "vbase.h"

/* Handle to lock free queue. */
typedef void* vqueue_t;

/*
** Gets the amount of memory required by queue of the specified size.
** @param node_count Maximum of nodes in the queue.
** @return The amount of memory required.
** @see vqueue_create
*/
size_t vqueue_get_bytes_required(int node_count);

/*
** Create a lock free queue.
** @param buffer A buffer of size vqueue_get_bytes_required().
** @param node_count Maximum of nodes in the queue.
** @return A new lock free queue.
** @see vqueue_get_bytes_required
*/
vqueue_t vqueue_create(void* buffer, int node_count);

/*
** Push data onto a queue.
** @param queue The queue on which to push the data.
** @param data The data to push on the queue.
** @see vqueue_pop
*/
void vqueue_push(vqueue_t queue, void* data);

/*
** Pop data from a queue.
** @param queue The queue to pop data off.
** @param data On successful return, pointer to data popped.
** @return If the queue was not empty, true is returned.
** @see vqueue_push
*/
bool vqueue_pop(vqueue_t queue, void** data);

/*
** Get the number of items in the queue.
*/
int vqueue_get_count(vqueue_t q);
