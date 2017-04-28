/*
    Copyright (c) 2016-2017 Teddy ASTIE (TSnake41)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef H_FLLIST
#define H_FLLIST

#include <limits.h>

/**
 * Out of memory handler.
 */
extern void (*ll_oom_handler)(void);

typedef struct linked_list {
    struct linked_list *next;
    void *value;
} llist_t, queue_t, stack_t;

/**
 * Insert an item at index to the linked list.
 */
llist_t *ll_insert(llist_t *llist, void *value, unsigned int index);

/**
 * Insert an item at the beginning of the linked list.
 */
#define ll_insert_begin(llist, value) ll_insert(llist, value, 0)

/**
 * Insert an item at the end of the linked list.
 */
#define ll_insert_end(llist, value) ll_insert(llist, value, UINT_MAX)

/**
 * Remove a node from a list.
 */
llist_t *ll_remove(llist_t *llist, llist_t *node);

/**
 * Remove a node using its id from a list.
 */
#define ll_remove_at(llist, index) ll_remove(llist, ll_get_node(llist, index))

/**
 * Get an item's node using its index.
 */
llist_t *ll_get_node(llist_t *llist, unsigned int index);

/**
 * Get an item's value using its index.
 */
#define ll_get_value(llist, index) (ll_get_node(llist, index)->value)


/**
 * Get the count of items in the linked list.
 */
unsigned int ll_get_count(llist_t *llist);

/**
 * Go a value in the queue.
 */
#define queue_push(queue, value) ll_insert_end(queue, value)

/**
 * Pull the next value of the node.
 */
llist_t *queue_pop(llist_t *llist);

/**
 * Get the size of the queue.
 */
#define queue_size(queue) ll_get_count(queue)

/**
 * Push a value to the stack.
 */
#define stack_push(stack, value) ll_insert_begin(stack, value)

/**
 * Pop a value upon the stack.
 */
#define stack_pop(stack) queue_pop(stack)

/**
 * Get the size of the stack.
 */
#define stack_size(stack) ll_get_count(stack)

#endif /* H_LLIST */
