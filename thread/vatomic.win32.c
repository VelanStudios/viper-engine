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
*/

#include "thread/vatomic.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

int32_t vatomic32_compare_exchange(int32_t* store, int32_t comp, int32_t value)
{
	return _InterlockedCompareExchange((volatile long*)store, value, comp);
}

int32_t vatomic32_exchange(int32_t* store, int32_t value)
{
	return _InterlockedExchange((volatile long*)store, value);
}

int32_t vatomic32_exchange_add(int32_t* store, int32_t value)
{
	return _InterlockedExchangeAdd((volatile long*)store, value);
}

int32_t vatomic32_increment(int32_t* store)
{
	return _InterlockedIncrement((volatile long*)store) - 1;
}

int32_t vatomic32_decrement(int32_t* store)
{
	return _InterlockedDecrement((volatile long*)store) + 1;
}

int64_t vatomic64_compare_exchange(int64_t* store, int64_t comp, int64_t value)
{
	return _InterlockedCompareExchange64((volatile __int64*)store, value, comp);
}

int64_t vatomic64_exchange(int64_t* store, int64_t value)
{
	return _InterlockedExchange64((volatile __int64*)store, value);
}

int64_t vatomic64_exchange_add(int64_t* store, int64_t value)
{
	return _InterlockedExchangeAdd64((volatile __int64*)store, value);
}

int64_t vatomic64_increment(int64_t* store)
{
	return _InterlockedIncrement64((volatile __int64*)store) - 1;
}

int64_t vatomic64_decrement(int64_t* store)
{
	return _InterlockedDecrement64((volatile __int64*)store) + 1;
}

void vatomic_barrier()
{
	int32_t unused = 0;
	vatomic32_exchange(&unused, 1);
}
