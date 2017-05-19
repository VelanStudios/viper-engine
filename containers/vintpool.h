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
** Lock free pool.
*/

#include "vbase.h"

/* Handle to lock free pool. */
typedef void* vintpool_t;

/*
** Gets the amount of memory required by an integer pool of the specified size.
** @param index_count Number of indices in the pool.
** @return The amount of memory required.
** @see vintpool_create
*/
size_t vintpool_get_bytes_required(int index_count);

/*
** Create a lock free integer pool.
** @param buffer A buffer of size vintpool_get_bytes_required().
** @param index_count Number of indices in the pool.
** @return A new integer pool.
** @see vintpool_get_memory_size
*/
vintpool_t vintpool_create(void* buffer, int index_count);

/*
** Allocate an index from the pool. Spins until an index is free.
** @param pool The pool to allocate from.
** @return A new index.
** @see vintpool_free
*/
int vintpool_alloc(vintpool_t pool);

/*
** Free previously allocated pool index.
** @param pool The pool where the index was previously allocated.
** @param index The index to free.
** @see vintpool_alloc
*/
void vintpool_free(vintpool_t pool, int index);

/*
** Gets the number of indices in the pool.
** @see vintpool_create
*/
int vintpool_get_index_count(vintpool_t p);
