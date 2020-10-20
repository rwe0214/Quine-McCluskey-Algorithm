#include "lists.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void add_entry(node_t **head, uint new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->val = new_value;
    new_node->next = NULL;

    assert(new_node);
    if (*indirect)
        new_node->next = *indirect;
    *indirect = new_node;
}

node_t *find_entry(node_t *head, uint val)
{
    node_t *current = head;
    for (; current && current->val != val; current = current->next)
        ;
    return current;
}

void del_entry(node_t **head, node_t *target)
{
    if (!target) {
        fprintf(stderr, "[warning] delete target is NULL\n");
        return;
    }
    node_t **indirect = head;
    while (*indirect != target)
        indirect = &(*indirect)->next;
    *indirect = target->next;
    free(target);
    target = NULL;
}

void print_list(node_t *head)
{
    for (node_t *iter = head; iter; iter = iter->next)
        printf("%d ", iter->val);
    printf("\n");
}

void free_list(node_t *head)
{
    head->next ? free_list(head->next) : free(head);
    head = NULL;
}
