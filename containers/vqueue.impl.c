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

#include "containers/vqueue.h"

#include "thread/vatomic.h"

typedef struct _vqueue_nodecount_t
{
	uint32_t index;
	uint32_t count;
} vqueue_nodecount_t;

typedef union _vqueue_pointer_t
{
	uint64_t entire;
	vqueue_nodecount_t part;
} vqueue_pointer_t;

typedef struct _vqueue_node_t
{
	void* data;
	vqueue_pointer_t next;
} vqueue_node_t;

typedef struct _vqueue_impl_t
{
	vqueue_pointer_t head;
	vqueue_pointer_t tail;
	vqueue_pointer_t free_list;

	int32_t count;

	vqueue_node_t* nodes;
} vqueue_impl_t;

static const uint32_t k_vqueue_invalid_index = 0xffffffff;

static uint32_t _alloc_node_index(vqueue_impl_t* queue);
static void _free_node_index(vqueue_impl_t* queue, uint32_t index);
static vqueue_node_t* _init_node(vqueue_impl_t* queue, uint32_t node_index);

size_t vqueue_get_bytes_required(int node_count)
{
	return sizeof(vqueue_impl_t) + (sizeof(vqueue_node_t) * node_count);
}

vqueue_t vqueue_create(void* buffer, int node_count)
{
	vqueue_impl_t* queue = (vqueue_impl_t*)buffer;

	queue->count = 0;
	queue->free_list.entire = 0;
	queue->nodes = (vqueue_node_t*)(queue + 1);

	/* Link nodes together. */
	for (int i = 0; i < node_count - 1; ++i)
	{
		queue->nodes[i].next.part.index = i + 1;
		queue->nodes[i].next.part.count = 0;
	}
	queue->nodes[node_count - 1].next.part.index = k_vqueue_invalid_index;
	queue->nodes[node_count - 1].next.part.count = 0;

	/* Populate the queue with a dummy node. */
	uint32_t dummy_index = _alloc_node_index(queue);
	_init_node(queue, dummy_index);

	queue->head.part.index = dummy_index;
	queue->head.part.count = 0;
	queue->tail.part.index = dummy_index;
	queue->tail.part.count = 0;

	return queue;
}

void vqueue_push(vqueue_t q, void* data)
{
	vqueue_impl_t* queue = (vqueue_impl_t*)(q);

	/* Allocate a new node for this data. */
	uint32_t node_index = _alloc_node_index(queue);
	vqueue_node_t* node = _init_node(queue, node_index);
	node->data = data;

	vqueue_pointer_t tail;

	/* Try until the push succeeds. */
	for (;;)
	{
		tail = queue->tail;
		vqueue_pointer_t next = queue->nodes[tail.part.index].next;

		/* Is our view of the queue still consistent? If not, try again. */
		if (tail.entire == queue->tail.entire)
		{
			/* Is tail pointing to last node? */
			if (next.part.index == k_vqueue_invalid_index)
			{
				/* Attempt to push new node onto tail. Leave the loop on success. */
				vqueue_pointer_t link = { .part.index = node_index, .part.count = next.part.count + 1 };
				if (vatomic64_compare_exchange(&queue->nodes[tail.part.index].next.entire, next.entire, link.entire) == next.entire)
				{
					break;
				}
			}

			/* Tail has fallen behind the actual end of the queue. Fix that. */
			else
			{
				vqueue_pointer_t link = { .part.index = next.part.index, .part.count = tail.part.count + 1 };
				vatomic64_compare_exchange(&queue->tail.entire, tail.entire, link.entire);
			}
		}
	}

	/* Try to advance the tail pointer. We'll handle the fail case on future calls. */
	{
		vqueue_pointer_t link = { .part.index = node_index, .part.count = tail.part.count + 1 };
		vatomic64_compare_exchange(&queue->tail.entire, tail.entire, link.entire);
		vatomic32_increment(&queue->count);
	}
}

bool vqueue_pop(vqueue_t q, void** data)
{
	vqueue_impl_t* queue = (vqueue_impl_t*)(q);
	vqueue_pointer_t head;

	for (;;)
	{
		head = queue->head;
		vqueue_pointer_t tail = queue->tail;
		vqueue_pointer_t next = queue->nodes[head.part.index].next;

		/* Is our view of the queue still consistent? If not, try again. */
		if (head.entire == queue->head.entire)
		{
			if (head.part.index == tail.part.index)
			{
				/* If queue is empty, fail the pop. */
				if (next.part.index == k_vqueue_invalid_index)
				{
					return false;
				}

				/* Tail has fallen behind the actual end of the queue. Fix that. */
				vqueue_pointer_t link = { .part.index = next.part.index, .part.count = tail.part.count + 1 };
				vatomic64_compare_exchange(&queue->tail.entire, tail.entire, link.entire);
			}
			else
			{
				/* Grab the data. */
				*data = queue->nodes[next.part.index].data;

				/* Attempt to pop the node. Leave the loop on success. */
				vqueue_pointer_t link = { .part.index = next.part.index, .part.count = head.part.count + 1 };
				if (vatomic64_compare_exchange(&queue->head.entire, head.entire, link.entire) == head.entire)
				{
					break;
				}
			}
		}
	}

	vatomic32_decrement(&queue->count);
	_free_node_index(queue, head.part.index);
	return true;
}

int vqueue_get_count(vqueue_t q)
{
	vqueue_impl_t* queue = (vqueue_impl_t*)(q);
	return queue->count;
}

static uint32_t _alloc_node_index(vqueue_impl_t* queue)
{
	uint32_t index;

	for (;;)
	{
		vqueue_pointer_t free_list = queue->free_list;

		if (free_list.part.index != k_vqueue_invalid_index)
		{
			index = free_list.part.index;
			vqueue_pointer_t next = queue->nodes[index].next;

			vqueue_pointer_t link = { .part.index = next.part.index, .part.count = free_list.part.count + 1 };
			if (vatomic64_compare_exchange(&queue->free_list.entire, free_list.entire, link.entire) == free_list.entire)
			{
				break;
			}
		}
	}

	return index;
}

static void _free_node_index(vqueue_impl_t* queue, uint32_t index)
{
	vqueue_node_t* node = queue->nodes + index;
	for (;;)
	{
		vqueue_pointer_t free_list = queue->free_list;
		node->next.part.index = free_list.part.index;

		vqueue_pointer_t link = { .part.index = index, .part.count = free_list.part.count + 1 };
		if (vatomic64_compare_exchange(&queue->free_list.entire, free_list.entire, link.entire) == free_list.entire)
		{
			break;
		}
	}
}

static vqueue_node_t* _init_node(vqueue_impl_t* queue, uint32_t node_index)
{
	vqueue_node_t* node = queue->nodes + node_index;
	node->data = 0;
	node->next.part.index = k_vqueue_invalid_index;
	node->next.part.count = 0;

	vatomic_barrier();

	return node;
}
