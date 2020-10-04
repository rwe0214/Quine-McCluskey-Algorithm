#ifndef __UTIL_H
#define __UTIL_H

typedef struct __node {
    int val;
    struct __node *prev;
    struct __node *next;
} node_t;

typedef struct __list_head {
    struct __node *head;
    struct __node *tail;
} list_head;

#define HASH_TAB_SIZE 32

void add_entry(node_t **, int);
void print_list(node_t *);
void free_list(node_t *);


#endif