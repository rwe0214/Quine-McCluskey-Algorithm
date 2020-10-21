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

/*
 * node index start from 0
 * this function did not handle the circustance when k is larger than list
 * length
 */
uint find_kth_entry(node_t *head, uint k)
{
    node_t *iter;
    for (iter = head; iter && k > 0; iter = iter->next, k--)
        ;
    return iter->val;
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

/*
 * node index start from 0
 * this function did not handle the circustance when k is larger than list
 * length
 */
void del_kth_entry(node_t **head, int k)
{
    node_t **indirect = head;
    while (*indirect && k > 0) {
        indirect = &(*indirect)->next;
        k--;
    }
    node_t *target = *indirect;
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
    // if(head->next)
    //     free_list(head->next);
    // free(head);
    // head = NULL;
    while (head) {
        node_t *tmp = head;
        head = head->next;
        free(tmp);
        tmp = NULL;
    }
}

int sizeof_list(node_t *head)
{
    int count = 0;
    for (node_t *iter = head; iter; iter = iter->next)
        count++;
    return count;
}