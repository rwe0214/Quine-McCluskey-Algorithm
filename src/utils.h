#ifndef __UTILS_H
#define __UTILS_H

/*
 * 32 bit integer are divided into the five parts:
 * 31       30          30-24       23      22-20       19-10       9-0
 * merged   reduced     none-used   set     var_nums    dc          implicant
 */

#define MASK_EXPRESSION 0x000003ff
#define MASK_DC_STAT 0x000ffc00
#define MASK_VARNUMS 0x00700000
#define MASK_SETBIT 0x00800000
#define MASK_REDUCED 0x40000000
#define MASK_MERGED 0x80000000

#define DC_STAT_POS 10
#define VARNUMS_POS 20
#define SET_POS 23
#define REDUCED_POS 30
#define MERGED_POS 31

#define init_list(head) head = NULL;

typedef unsigned int uint;
typedef struct __node {
    uint val;
    struct __node *next;
} node_t;

void add_entry(node_t **, uint);
node_t *find_entry(node_t *, uint);
void del_entry(node_t **, node_t *);
void print_list(node_t *);
void free_list(node_t *);

void print_minterm(uint, int, int);


#endif
