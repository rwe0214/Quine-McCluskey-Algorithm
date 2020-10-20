#include "mccluskey.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void init_hmap(node_t ***hmap, int n)
{
    *hmap = malloc((n + 1) * sizeof(node_t *));
    assert(*hmap);
    for (int i = 0; i <= n; i++) {
        init_list((*hmap)[i]);
    }
}

void add_hmap(node_t ***hmap, node_t *list)
{
    for (node_t *iter = list; iter; iter = iter->next) {
        int h_id = count_set_bit(iter->val & MASK_EXPRESSION);
        add_entry(&(*hmap)[h_id], iter->val);
    }
}

void hmap_build(node_t ***hmap, node_t *sets, node_t *dcs, int var_nums)
{
    init_hmap(hmap, var_nums);
    add_hmap(hmap, sets);
    add_hmap(hmap, dcs);
}

int is_merged(uint a)
{
    return (a & MASK_MERGED) >> MERGED_POS;
}

int is_mergeable(uint a, uint b)
{
    if (diff_bits(a & MASK_EXPRESSION, b & MASK_EXPRESSION) == 1 &&
        pattern_is_equal(a & MASK_DC_STAT, b & MASK_DC_STAT))
        return 1;
    return 0;
}

uint merge_minterms(uint a, uint b)
{
    uint m = 0;
    int diff_bit = ffs((a & MASK_EXPRESSION) ^ (b & MASK_EXPRESSION)) - 1;

    m |= ((a & MASK_EXPRESSION) & (MASK_EXPRESSION ^ (1 << diff_bit)));
    m |=
        ((a & MASK_DC_STAT) & (MASK_DC_STAT ^ (1 << (diff_bit + DC_STAT_POS))));
    return m;
}

int pattern_is_equal(uint a, uint b)
{
    return diff_bits(a, b) ? 0 : 1;
}

int diff_bits(uint a, uint b)
{
    return count_set_bit(a ^ b);
}

int count_set_bit(int n)
{
    int count = 0;
    while (n) {
        count++;
        n &= n - 1;
    }
    return count;
}

void print_hmap(node_t ***hmap, int n)
{
    for (int i = 0; i < n; i++) {
        if (hmap[i]) {
            printf("hmap[%d]:\n", i);
            print_hmap_col(hmap[i], n);
        }
    }
}

void print_hmap_col(node_t **row, int n)
{
    for (int i = 0; i <= n; i++) {
        printf("\t[%d]: ", i);
        print_list_minterm(row[i], n, 0);
    }
}

void free_map(node_t ***map, int n)
{
    for (int i = 0; i < n; i++) {
        if (map[i])
            free_map_col(&map[i], n);
    }
}

void free_map_col(node_t ***col, int n)
{
    for (int i = 0; i < n; i++)
        if ((*col)[i])
            free_list((*col)[i]);
    free(*col);
    *col = NULL;
}

void print_list_minterm(node_t *head, int n, int var_style)
{
    for (node_t *iter = head; iter; iter = iter->next) {
        print_minterm(iter->val, n, var_style);
        printf(" ");
    }
    printf("\n");
}

void print_minterm(uint encode, int n, int var_style)
{
    uint exp = (encode & MASK_EXPRESSION);
    uint dc = (encode & MASK_DC_STAT) >> DC_STAT_POS;
    char VARIABLE[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    for (int i = n - 1; i >= 0; i--) {
        if (dc & (1 << i)) {
            if (exp & (1 << i)) {
                (var_style) ? printf("%c", VARIABLE[n - 1 - i]) : printf("1");
            } else {
                (var_style) ? printf("%c\'", VARIABLE[n - 1 - i]) : printf("0");
            }
        } else if (!var_style) {
            printf("-");
        }
    }
}

void run_mccluskey(node_t ***hmap, int n){
    for (int i = 0; i < n; i++) {
        node_t *tmp_list;
        init_hmap(&hmap[i + 1], n);
        init_list(tmp_list);
        int stop = 1;
        for (int j = 0; j < n; j++) {
            printf("[%d][%d]\n", i, j);
            for (node_t *a = hmap[i][j]; a; a = a->next) {
                printf("\ta = ");
                print_minterm(a->val, 4, 0);
                printf("\n");
                node_t *b = hmap[i][j + 1];
                while (b) {
                    printf("\tfind b = ");
                    print_minterm(b->val, 4, 0);
                    printf("\n");
                    if (is_mergeable(a->val, b->val)) {
                        a->val |= MASK_MERGED;
                        b->val |= MASK_MERGED;
                        uint m = merge_minterms(a->val, b->val);
                        if (!find_entry(tmp_list, m)) {
                            printf("\tMERGE ADD!\n");
                            stop = 0;
                            add_entry(&tmp_list, m);
                        }
                    }
                    b = b->next;
                }
                if (!is_merged(a->val)) {
                    printf("\tNONE MERGE ADD!\n");
                    add_entry(&tmp_list, a->val);
                }
            }
        }
        fprintf(stderr, "############ end ############\n");
        if (stop) {
            free_map_col(&hmap[i + 1], n + 1);
            free_list(tmp_list);
            break;
        } else {
            add_hmap(&hmap[i + 1], tmp_list);
            free_list(tmp_list);
        }
    }
}