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
extern void (*list_oom_handler)(void);

typedef struct list {
    struct list *next;
    void *value;
} list_t, queue_t, stack_t;

/**
 * Insert an item at index to the list.
 */
list_t *list_insert(list_t *llist, void *value, unsigned int index);

/**
 * Insert an item at the beginning of the list.
 */
#define list_insert_begin(llist, value) list_insert(list, value, 0)

/**
 * Insert an item at the end of the list.
 */
#define list_insert_end(llist, value) list_insert(list, value, UINT_MAX)

/**
 * Remove a node from a list.
 */
list_t *list_remove(list_t *llist, list_t *node);

/**
 * Remove a node using its id from a list.
 */
#define list_remove_at(list, index) list_remove(list, list_get_node(list, index))

/**
 * Get an item's node using its index.
 */
list_t *list_get_node(list_t *list, unsigned int index);

/**
 * Get an item's value using its index.
 */
#define list_get_value(list, index) (list_get_node(list, index)->value)


/**
 * Get the count of items in the list.
 */
unsigned int list_get_count(list_t *list);

/**
 * Go a value in the queue.
 */
#define queue_push(queue, value) list_insert_end(queue, value)

/**
 * Pull the next value of the node.
 */
list_t *queue_pop(list_t *list);

/**
 * Get the size of the queue.
 */
#define queue_size(queue) list_get_count(queue)

/**
 * Push a value to the stack.
 */
#define stack_push(stack, value) list_insert_begin(stack, value)

/**
 * Pop a value upon the stack.
 */
#define stack_pop(stack) queue_pop(stack)

/**
 * Get the size of the stack.
 */
#define stack_size(stack) list_get_count(stack)

#endif /* H_LLIST */
