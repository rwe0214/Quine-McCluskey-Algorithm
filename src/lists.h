#ifndef __LISTS_H
#define __LISTS_H

#define init_list(head) head = NULL;

typedef unsigned int uint;
typedef struct __node {
    uint val;
    struct __node *next;
} node_t;

void add_entry(node_t **, uint);
uint find_kth_entry(node_t *, uint);
node_t *find_entry(node_t *, uint);
void del_entry(node_t **, node_t *);
void del_kth_entry(node_t **, int);
void print_list(node_t *);
void free_list(node_t *);
int sizeof_list(node_t *);

#endif
