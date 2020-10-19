#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void add_pentry(node_p **head, uint *new_value_addr)
{
    node_p **indirect = head;

    node_p *new_node = malloc(sizeof(node_p));
    new_node->ptr = new_value_addr;
    new_node->next = NULL;

    assert(new_node);
    if (*indirect) {
        new_node->next = *indirect;
    }
    *indirect = new_node;
}

node_p *find_pentry(node_p *head, uint *val_addr){
    node_p *current = head;
    for (; current && current->ptr != val_addr; current = current->next);
    return current;
}

void del_pentry(node_p **head, node_p *target){
    if(!target){
        fprintf(stderr, "[warning] delete target is NULL\n");
        return;
    }
    node_p **indirect = head;
    while(*indirect != target)
        indirect = &(*indirect)->next;
    *indirect = target->next;
    free(target);
}

void print_plist(node_p *head)
{
    for (node_p *iter = head; iter; iter = iter->next)
        printf("%d:%d ", *(iter->ptr) & MASK_VALUEBITS, (*(iter->ptr) & MASK_SETBIT) >> SET_BIT);
    printf("\n");
}

void free_plist(node_p *head)
{
    head->next ? free_plist(head->next) : free(head);
}

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

node_t *find_entry(node_t *head, uint val){
    node_t *current = head;
    for (; current && current->val != val; current = current->next);
    return current;
}

void del_entry(node_t **head, node_t *target){
    if(!target){
        fprintf(stderr, "[warning] delete target is NULL\n");
        return;
    }
    node_t **indirect = head;
    while(*indirect != target)
        indirect = &(*indirect)->next;
    *indirect = target->next;
    free(target);
}

void print_list(node_t *head)
{
    for (node_t *iter = head; iter; iter = iter->next)
        printf("%d ", iter->val & MASK_VALUEBITS);
    printf("\n");
}

void free_list(node_t *head)
{
    head->next ? free_list(head->next) : free(head);
}

void print_minterm(uint encode, int n, int var_style){
    uint exp = (encode & MASK_VALUEBITS);
    uint dc = (encode & MASK_DCBITS) >> DC_BIT;
    char VARIABLE[10] = {'A','B','C','D','E','F','G','H','I','J'};
    for(int i=n-1; i>=0; i--){
        if (dc & (1 << i)){
            if (exp & (1 << i)){
                (var_style)?printf("%c", VARIABLE[n-1-i]):printf("1");
            }
            else{
                (var_style)?printf("%c\'", VARIABLE[n-1-i]):printf("0");
            }
        }
    }
}