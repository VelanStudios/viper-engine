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

#include "containers/vintpool.h"

#include "thread/vatomic.h"

typedef struct _vintpool_nodecount_t
{
	uint32_t index;
	uint32_t count;
} vintpool_nodecount_t;

typedef union _vintpool_pointer_t
{
	uint64_t entire;
	vintpool_nodecount_t part;
} vintpool_pointer_t;

typedef struct _vintpool_node_t
{
	vintpool_pointer_t next;
} vintpool_node_t;

typedef struct _vintpool_impl_t
{
	int index_count;

	vintpool_pointer_t free_list;

	vintpool_node_t* nodes;
} vintpool_impl_t;

static const uint32_t k_vintpool_invalid_index = 0xffffffff;

size_t vintpool_get_bytes_required(int index_count)
{
	return sizeof(vintpool_impl_t) + (sizeof(vintpool_node_t) * index_count);
}

vintpool_t vintpool_create(void* buffer, int index_count)
{
	vintpool_impl_t* pool = (vintpool_impl_t*)buffer;

	pool->index_count = index_count;
	pool->free_list.entire = 0;
	pool->nodes = (vintpool_node_t*)(pool + 1);

	for (int i = 0; i < index_count - 1; ++i)
	{
		pool->nodes[i].next.part.index = i + 1;
		pool->nodes[i].next.part.count = 0;
	}
	pool->nodes[index_count - 1].next.part.index = k_vintpool_invalid_index;
	pool->nodes[index_count - 1].next.part.count = 0;

	return pool;
}

int vintpool_alloc(vintpool_t p)
{
	int index;
	vintpool_impl_t* pool = (vintpool_impl_t*)(p);

	for (;;)
	{
		vintpool_pointer_t free_list = pool->free_list;

		if (free_list.part.index != k_vintpool_invalid_index)
		{
			index = free_list.part.index;
			vintpool_pointer_t next = pool->nodes[index].next;

			vintpool_pointer_t link = { .part.index = next.part.index, .part.count = free_list.part.count + 1 };
			if (vatomic64_compare_exchange(&pool->free_list.entire, free_list.entire, link.entire) == free_list.entire)
			{
				break;
			}
		}
	}

	return index;
}

void vintpool_free(vintpool_t p, int index)
{
	vintpool_impl_t* pool = (vintpool_impl_t*)(p);

	vintpool_node_t* node = pool->nodes + index;
	for (;;)
	{
		vintpool_pointer_t free_list = pool->free_list;
		node->next.part.index = free_list.part.index;

		vintpool_pointer_t link = { .part.index = index, .part.count = free_list.part.count + 1 };
		if (vatomic64_compare_exchange(&pool->free_list.entire, free_list.entire, link.entire) == free_list.entire)
		{
			break;
		}
	}
}

int vintpool_get_index_count(vintpool_t p)
{
	vintpool_impl_t* pool = (vintpool_impl_t*)(p);
	return pool->index_count;
}
