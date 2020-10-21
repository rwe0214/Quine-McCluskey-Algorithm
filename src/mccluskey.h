#ifndef __MCCCLUSKEY_H
#define __MCCCLUSKEY_H

#include <stdbool.h>
#include <stdio.h>

#include "lists.h"
/*
 * 32 bit integer are divided into the five parts:
 * 31       30-20       19-10       9-0
 * merged   none-used   dc          implicant
 */

#define MASK_EXPRESSION 0x000003ff
#define MASK_DC_STAT 0x000ffc00
#define MASK_NO_USE 0x7ff00000
#define MASK_MERGED 0x80000000

#define EXPRESSION_BIT 0
#define DC_STAT_POS 10
#define MERGED_POS 31

typedef struct SET {
    uint name;
    uint *eles;
    bool *covered;
    int size;
    float cost;
} set;

void init_hmap_col(node_t ***, int);
void add_hmap_col(node_t ***, node_t *);
void build_hmap_col(node_t ***, node_t *, node_t *, int);

int is_merged(uint);
int is_mergeable(uint, uint);
int pattern_is_equal(uint, uint);
int diff_bits(uint, uint);
int count_set_bit(int);

void run_mccluskey(node_t ***, int, node_t **);
void find_minimal_cover(node_t **, node_t **, node_t **, int);
uint merge_minterms(uint, uint);
int cost_minterms(node_t *, int);

set appr_min_set_cover(set, set *, int);
int compare_float(const void *, const void *);
bool is_covered(uint, uint);
bool **build_cover_matrix(node_t *, node_t *, int, int);

void print_hmap(node_t ***, int, FILE *);
void print_hmap_col(node_t **, int, FILE *);
void print_list_minterm(node_t *, int, int, char, FILE *);
void print_minterm(uint, int, int, FILE *);

void free_map(node_t ***, int);
void free_map_col(node_t ***, int);
void free_2d_array(void **, int);
void free_set_content(set *);
void free_sets(set *, int);

#endif
