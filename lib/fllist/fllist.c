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

/* Fast simple linked list implementation */

#include <stdlib.h>
#include <stdbool.h>
#include <fllist.h>

void (*ll_oom_handler)(void) = NULL;

llist_t *ll_insert(llist_t *llist, void *value, unsigned int index)
{
    llist_t *base = llist;
    llist_t *node = calloc(sizeof(llist_t), 1);

    if (node == NULL)
        /* Allocation failed */
        if (ll_oom_handler != NULL)
            ll_oom_handler();

    node->value = value;

    if (base == NULL)
        return node;

    if (index == 0) {
        node->next = base;
        return node;
    }

    unsigned int i = 0;
    while (llist->next && (i++ < (index - 1)))
        llist = llist->next;

    if (llist->next != NULL)
        node->next = llist->next;

    llist->next = node;
    return base;
}

llist_t *ll_remove(llist_t *llist, llist_t *node)
{
    llist_t *base = llist;

    if (base == NULL)
        return NULL;

    if (node == NULL)
        return base;

    if (base == node) {
        llist = base->next;
        free(base);
        return llist;
    }

    while (llist->next) {
        if (llist->next == node) {
            llist_t *n_node = llist->next->next;
            free(llist->next);
            llist->next = n_node;
            break;
        }
        llist = llist->next;
    }

    return base;
}

llist_t *ll_get_node(llist_t *llist, unsigned int index)
{
    if (llist == NULL)
        return NULL;

    if (index == 0)
        return llist;

    unsigned int i = 0;
    while (llist->next && (i++ < index))
        llist = llist->next;

    return llist;
}

unsigned int ll_get_count(llist_t *llist)
{
    if (llist == NULL)
        return 0;

    unsigned int i = 1;

    while ((llist = llist->next) != NULL)
        i++;

    return i;
}

queue_t *queue_pop(queue_t *queue)
{
    if (!queue)
        return NULL;

    queue_t *next_queue = queue->next;
    free(queue);

    return next_queue;
}
