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
** Atomic integer functions.
*/

#include "vbase.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	** Compare two values atomically, and if equal, store a third value.
	** @param store Address of first value to compare, and destination storage if equal.
	** @param comp Second value to compare against.
	** @param value Value to store if *store == comp.
	** @return The previous value at *store.
	*/
	int32_t vatomic32_compare_exchange(int32_t* store, int32_t comp, int32_t value);

	/*
	** Store a value atomically.
	** @param store Destination storage.
	** @param value Value to store.
	** @return The previous value at *store.
	*/
	int32_t vatomic32_exchange(int32_t* store, int32_t value);

	/*
	** Add two numbers atomically.
	** @param store Destination storage.
	** @param value Value to add to *store.
	** @return The previous value at *store.
	*/
	int32_t vatomic32_exchange_add(int32_t* store, int32_t value);

	/*
	** Increment a number atomically.
	** @param store Pointer to the number to increment.
	** @return The previous value at *store.
	*/
	int32_t vatomic32_increment(int32_t* store);

	/*
	** Decrement a number atomically.
	** @param store Pointer to the number to decrement.
	** @return The previous value at *store.
	*/
	int32_t vatomic32_decrement(int32_t* store);

	/*
	** Compare two values atomically, and if equal, store a third value.
	** @param store Address of first value to compare, and destination storage if equal.
	** @param comp Second value to compare against.
	** @param value Value to store if *store == comp.
	** @return The previous value at *store.
	*/
	int64_t vatomic64_compare_exchange(int64_t* store, int64_t comp, int64_t value);

	/*
	** Store a value atomically.
	** @param store Destination storage.
	** @param value Value to store.
	** @return The previous value at *store.
	*/
	int64_t vatomic64_exchange(int64_t* store, int64_t value);

	/*
	** Add two numbers atomically.
	** @param store Destination storage.
	** @param value Value to add to *store.
	** @return The previous value at *store.
	*/
	int64_t vatomic64_exchange_add(int64_t* store, int64_t value);

	/*
	** Increment a number atomically.
	** @param store Pointer to the number to increment.
	** @return The previous value at *store.
	*/
	int64_t vatomic64_increment(int64_t* store);

	/*
	** Decrement a number atomically.
	** @param store Pointer to the number to decrement.
	** @return The previous value at *store.
	*/
	int64_t vatomic64_decrement(int64_t* store);

	/*
	** Wait for all reads and write to complete, across all cores.
	*/
	void vatomic_barrier();

#ifdef __cplusplus
}
#endif
