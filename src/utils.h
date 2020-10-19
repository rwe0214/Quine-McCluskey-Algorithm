#ifndef __UTILS_H
#define __UTILS_H

/*
 * 32 bit integer are divided into the five parts:
 * 31       30          30-24       23      22-20       19-10       9-0
 * merged   reduced     none-used   set     var_nums    dc          implicant
 */

#define MASK_VALUEBITS 0x000003ff
#define MASK_DCBITS 0x000ffc00
#define MASK_VARNUMS 0x00700000
#define MASK_SETBIT 0x00800000
#define MASK_REDUCED 0x40000000
#define MASK_MERGED 0x80000000

#define DC_BIT 10
#define VARNUM_BIT 20
#define SET_BIT 23
#define REDUCED_BIT 30
#define MERGED_BIT 31

#define init_list(head) head = NULL;

typedef unsigned int uint;
typedef struct __node_p {
    uint *ptr;
    struct __node_p *next;
} node_p;

typedef struct __node {
    uint val;
    struct __node *next;
} node_t;

void add_pentry(node_p **, uint *);
node_p *find_pentry(node_p *, uint *);
void del_pentry(node_p **, node_p *);
void print_plist(node_p *);
void free_plist(node_p *);
void add_entry(node_t **, uint);
node_t *find_entry(node_t *, uint);
void del_entry(node_t **, node_t *);
void print_list(node_t *);
void free_list(node_t *);

void print_minterm(uint, int, int);


#endif