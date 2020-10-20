#ifndef __MCCCLUSKEY_H
#define __MCCCLUSKEY_H

#include "lists.h"

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


void init_hmap(node_t ***, int);
void add_hmap(node_t ***, node_t *);
void hmap_build(node_t ***, node_t *, node_t *, int);

void print_hmap(node_t ***, int);
void print_hmap_col(node_t **, int);

int is_merged(uint);
int is_mergeable(uint, uint);
uint merge_minterms(uint, uint);
int pattern_is_equal(uint, uint);
int diff_bits(uint, uint);
int count_set_bit(int);
void free_map_col(node_t ***, int);
void free_map(node_t ***, int);

void run_mccluskey(node_t ***, int);
void print_list_minterm(node_t *, int, int);
void print_minterm(uint, int, int);

#endif
