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

void (*list_oom_handler)(void) = NULL;

list_t *list_insert(list_t *list, void *value, unsigned int index)
{
    list_t *base = list;
    list_t *node = calloc(sizeof(list_t), 1);

    if (node == NULL) {
        /* Allocation failed */
        if (list_oom_handler != NULL)
            list_oom_handler();

        return NULL;
    }

    node->value = value;

    if (base == NULL)
        return node;

    if (index == 0) {
        node->next = base;
        return node;
    }

    unsigned int i = 0;
    while (list->next && (i++ < (index - 1)))
        list = list->next;

    if (list->next != NULL)
        node->next = list->next;

    list->next = node;
    return base;
}

list_t *list_remove(list_t *list, list_t *node)
{
    list_t *base = list;

    if (base == NULL)
        return NULL;

    if (node == NULL)
        return base;

    if (base == node) {
        list = base->next;
        free(base);
        return list;
    }

    while (list->next) {
        if (list->next == node) {
            list_t *n_node = list->next->next;
            free(list->next);
            list->next = n_node;
            break;
        }
        list = list->next;
    }

    return base;
}

list_t *list_get_node(list_t *list, unsigned int index)
{
    if (list == NULL)
        return NULL;

    if (index == 0)
        return list;

    unsigned int i = 0;
    while (list->next && (i++ < index))
        list = list->next;

    return list;
}

unsigned int list_get_count(list_t *llist)
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
