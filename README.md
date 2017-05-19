# viper-engine
A collection of loosely coupled game engine components, mostly written in C.

* containers/vintpool - Lock-free resource handle pool.
* containers/vqueue - Lock-free queue.
* thread/vatomic - Integer atomic operations wrapper.

## containers/vintpool

First, create a pool. The pool will require 8 bytes per resource:

    const int k_i_have_10_things = 10;
    void* pool_buffer = malloc(vintpool_get_bytes_required(k_i_have_10_things));
    vintpool_t pool = vintpool_create(pool_buffer, k_i_have_10_things);

Then, use the pool to allocate a resource. If there are no free integers in the pool, this call will spin until another thread frees an index:

    int resource_index = vintpool_alloc(pool);

Finally, when done with the resouce, return it to the pool:

    vintpool_free(pool, resource_index)

Pools are thread-safe and lock-free.

## containers/vqueue

First, create a queue. The queue will require 8 bytes per element:

    const int k_max_queue_size = 10;
    void* queue_buffer = malloc(vqueue_get_bytes_required(k_max_queue_size));
    vqueue_t queue = vqueue_create(queue_buffer, k_max_queue_size);

Now push items on the queue. The push operation will spin until the queue has space to add the new item:

    void* some_data = ...;
    vqueue_push(queue, some_data);

And pop items off the queue. The pop operation can fail if the queue is empty:

    void* data_from_top_of_queue;
    bool is_pop_success = vqueue_pop(queue, &data_from_top_of_queue);

Queues are thread-safe and lock-free.

## thread/vatomic

CPUs commonly support a set of primitive integer operations, called atomic operations, that cannot suffer from data races in a multiprocessor environment. The vatomic module is a simple wrapper around atomic operations for 32-bit and 64-bit integers. Supported operations include:

* `vatomic_exchange` - Store an integer.
* `vatomic_increment` and `vatomic_decrement` - Increment and decrement an integer.
* `vatomic_exchange_add` - Add two integers storing the result in the first integer.
* `vatomic_compare_exchange` - Compare two integers and store a value if equal.
