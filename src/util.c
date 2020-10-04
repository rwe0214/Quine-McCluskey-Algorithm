#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->val = new_value;
    new_node->prev = new_node->next = new_node;

    assert(new_node);
    if (*indirect) {
        new_node->next = *indirect;
        new_node->prev = (*indirect)->prev;
        (*indirect)->prev->next = new_node;
    }
    *indirect = new_node;
}

void print_list(node_t *head)
{
    node_t *current = head;
    do {
        printf("%d ", current->val);
        current = current->next;
    } while (current != head);
    printf("\n");
}

void free_list(node_t *head)
{
    if (head == head->next)
        free(head);
    else {
        node_t *current = head->next;
        node_t *next = head->next->next;
        for (; current != head; current = next, next = next->next)
            free(current);
    }
}
