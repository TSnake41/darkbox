/*
  Copyright (C) 2016-2017 Teddy ASTIE

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/* Fast simple linked list implementation */

#include <stdlib.h>
#include <stdbool.h>
#include <fllist.h>

void (*list_oom_handler)(void) = NULL;

fllist *list_insert(fllist *list, void *value, unsigned int index)
{
    fllist *base = list;
    fllist *node = calloc(sizeof(fllist), 1);

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

fllist *list_remove(fllist *list, fllist *node)
{
    fllist *base = list;

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
            fllist *n_node = list->next->next;
            free(list->next);
            list->next = n_node;
            break;
        }
        list = list->next;
    }

    return base;
}

fllist *list_get_node(fllist *list, unsigned int index)
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

unsigned int list_get_count(fllist *llist)
{
    if (llist == NULL)
        return 0;

    unsigned int i = 1;

    while ((llist = llist->next) != NULL)
        i++;

    return i;
}

flqueue *queue_pop(flqueue *queue)
{
    if (!queue)
        return NULL;

    flqueue *next_queue = queue->next;
    free(queue);

    return next_queue;
}
