#include "mccluskey.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void init_hmap_col(node_t ***hmap_col, int n)
{
    *hmap_col = malloc((n + 1) * sizeof(node_t *));
    assert(*hmap_col);
    for (int i = 0; i <= n; i++) {
        init_list((*hmap_col)[i]);
    }
}

void add_hmap_col(node_t ***hmap_col, node_t *list)
{
    for (node_t *iter = list; iter; iter = iter->next) {
        int h_id = count_set_bit(iter->val & MASK_EXPRESSION);
        add_entry(&(*hmap_col)[h_id], iter->val);
    }
}

void build_hmap_col(node_t ***hmap_col, node_t *sets, node_t *dcs, int var_nums)
{
    init_hmap_col(hmap_col, var_nums);
    add_hmap_col(hmap_col, sets);
    add_hmap_col(hmap_col, dcs);
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

void print_hmap(node_t ***hmap, int n, FILE *stream)
{
    for (int i = 0; i < n; i++) {
        if (hmap[i]) {
            fprintf(stream, "hmap[%d]:\n", i);
            print_hmap_col(hmap[i], n, stream);
        }
    }
}

void print_hmap_col(node_t **row, int n, FILE *stream)
{
    for (int i = 0; i <= n; i++) {
        fprintf(stream, "\t[%d]: ", i);
        print_list_minterm(row[i], n, 0, ' ', stream);
    }
}

void free_map(node_t ***map, int n)
{
    for (int i = 0; i < n; i++) {
        if (map[i]) {
            // there are n+1 list in each column
            free_map_col(&map[i], n + 1);
        }
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

void print_list_minterm(node_t *head,
                        int n,
                        int var_style,
                        char end,
                        FILE *stream)
{
    for (node_t *iter = head; iter; iter = iter->next) {
        print_minterm(iter->val, n, var_style, stream);
        fprintf(stream, "%c", end);
    }
}

void print_minterm(uint encode, int n, int var_style, FILE *stream)
{
    uint exp = (encode & MASK_EXPRESSION);
    uint dc = (encode & MASK_DC_STAT) >> DC_STAT_POS;
    char VARIABLE[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    for (int i = n - 1; i >= 0; i--) {
        if (dc & (1 << i)) {
            if (exp & (1 << i)) {
                (var_style) ? fprintf(stream, "%c", VARIABLE[n - 1 - i])
                            : fprintf(stream, "1");
            } else {
                (var_style) ? fprintf(stream, "%c\'", VARIABLE[n - 1 - i])
                            : fprintf(stream, "0");
            }
        } else if (!var_style) {
            fprintf(stream, "-");
        }
    }
}

void run_mccluskey(node_t ***hmap, int n, node_t **prime_implicants)
{
    fprintf(stderr, "\n########### start mccluskey ##########\n");
    for (int i = 0; i < n; i++) {
        fprintf(stderr, "############# iteration %d ############\n", i + 1);
        node_t *tmp_list;
        init_hmap_col(&hmap[i + 1], n);
        init_list(tmp_list);
        int stop = 1;
        for (int j = 0; j < n; j++) {
            fprintf(stderr, "[%d][%d]\n", i, j);
            for (node_t *a = hmap[i][j]; a; a = a->next) {
                fprintf(stderr, "\ta = ");
                print_minterm(a->val, 4, 0, stderr);
                fprintf(stderr, "\n");
                node_t *b = hmap[i][j + 1];
                while (b) {
                    fprintf(stderr, "\tfind b = ");
                    print_minterm(b->val, 4, 0, stderr);
                    fprintf(stderr, "\n");
                    if (is_mergeable(a->val, b->val)) {
                        a->val |= MASK_MERGED;
                        b->val |= MASK_MERGED;
                        uint m = merge_minterms(a->val, b->val);
                        if (!find_entry(tmp_list, m)) {
                            fprintf(stderr, "\tMERGE ADD!\n");
                            stop = 0;
                            add_entry(&tmp_list, m);
                        }
                    }
                    b = b->next;
                }
                if (!is_merged(a->val)) {
                    fprintf(stderr, "\tNONE MERGE ADD!\n");
                    add_entry(&tmp_list, a->val);
                }
            }
        }
        fprintf(stderr, "\n########### iteration %d end ##########\n", i + 1);
        if (stop) {
            free_map_col(&hmap[i + 1], n + 1);
            free_list(tmp_list);
            break;
        } else {
            add_hmap_col(&hmap[i + 1], tmp_list);
            free_list(tmp_list);
        }
    }
    for (int i = 0; i < n - 1; i++) {
        if (!hmap[i + 1]) {
            for (int j = n; j >= 0; j--) {
                if (hmap[i][j]) {
                    for (node_t *iter = hmap[i][j]; iter; iter = iter->next)
                        add_entry(prime_implicants, iter->val);
                }
            }
        }
    }
    fprintf(stderr, "################ end #################\n");
}

void find_minimal_cover(node_t **cover, node_t **pis, node_t **cares, int n)
{
    fprintf(stderr, "\n####### find minimal set cover #######\n");
    int care_num = sizeof_list(*cares), pi_num = sizeof_list(*pis);
    bool **terms_covered = build_cover_matrix(*cares, *pis, care_num, pi_num);

    // find essential prime implicants
    node_t *covered_minterm;
    init_list(covered_minterm);
    for (int j = care_num - 1; j >= 0; j--) {
        int count = 0;
        for (int i = pi_num - 1; i >= 0; i--) {
            count += terms_covered[i][j];
        }
        if (count == 1) {
            for (int i = pi_num - 1; i >= 0; i--) {
                if (terms_covered[i][j]) {
                    add_entry(cover, find_kth_entry(*pis, i));
                    for (int k = care_num - 1; k >= 0; k--) {
                        if (terms_covered[i][k]) {
                            uint minterm = find_kth_entry(*cares, k);
                            if (!find_entry(covered_minterm, minterm))
                                add_entry(&covered_minterm, minterm);
                        }
                    }
                    break;
                }
            }
        }
    }

    for (node_t *iter = covered_minterm; iter; iter = iter->next) {
        node_t *target = find_entry(*cares, iter->val);
        del_entry(cares, target);
    }
    for (node_t *iter = *cover; iter; iter = iter->next) {
        node_t *target = find_entry(*pis, iter->val);
        del_entry(pis, target);
    }

    /* free old cover matrix */
    free_2d_array((void **) terms_covered, pi_num);


    care_num = sizeof_list(*cares);
    pi_num = sizeof_list(*pis);
    fprintf(stderr, "-------------------------------------\n");
    terms_covered = build_cover_matrix(*cares, *pis, care_num, pi_num);


    // mapping to minimal set cover problem
    set U, *S, min_set;
    U.eles = malloc(care_num * sizeof(uint));
    U.covered = malloc(care_num * sizeof(bool));
    U.size = care_num;
    S = malloc(pi_num * sizeof(set));
    for (int i = 0; i < care_num; i++) {
        U.eles[i] = find_kth_entry(*cares, i);
        U.covered[i] = false;
    }
    for (int i = 0; i < pi_num; i++) {
        S[i].name = find_kth_entry(*pis, i);
        S[i].eles = malloc(care_num * sizeof(uint));
        S[i].covered = NULL;
        int k = 0;
        for (int j = 0; j < care_num; j++) {
            if (terms_covered[i][j]) {
                (S[i].eles)[k++] = U.eles[j];
            }
        }
        S[i].size = (k) ? k : -1;
        S[i].cost = n - diff_bits((find_kth_entry(*pis, i) & MASK_DC_STAT),
                                  MASK_DC_STAT);
        S[i].cost /= S[i].size;
    }

    // solve minimal set cover problem
    min_set = appr_min_set_cover(U, S, pi_num);
    for (int i = 0; i < min_set.size; i++)
        add_entry(cover, min_set.eles[i]);

    fprintf(stderr, "################ end #################\n");
    free_list(covered_minterm);
    free_2d_array((void **) terms_covered, pi_num);
    free_set_content(&U);
    free_set_content(&min_set);
    free_sets(S, pi_num);
}

void free_sets(set *a, int size)
{
    for (int i = 0; i < size; i++) {
        free_set_content(a + i);
    }
    free(a);
    a = NULL;
}

void free_set_content(set *a)
{
    if (a->eles)
        free(a->eles);
    if (a->covered)
        free(a->covered);
    a->eles = NULL;
    a->covered = NULL;
}

void free_2d_array(void **ptr, int rows)
{
    for (int i = 0; i < rows; i++) {
        free(ptr[i]);
        ptr[i] = NULL;
    }
    free(ptr);
    ptr = NULL;
}

set appr_min_set_cover(set u, set *s, int n)
{
    set min_set;
    min_set.eles = malloc(n * sizeof(uint));
    min_set.covered = NULL;
    qsort(s, n, sizeof(set), compare_float);
    int l = 0;
    for (int i = 0; i < n; i++) {
        if (s[i].cost <= 0)
            continue;

        bool add_set = false;
        for (int j = 0; j < s[i].size; j++) {
            for (int k = 0; k < u.size; k++) {
                if (!u.covered[k] &&
                    pattern_is_equal(u.eles[k], (s[i].eles)[j])) {
                    add_set = u.covered[k] = true;
                    break;
                }
            }
        }

        if (add_set)
            min_set.eles[l++] = s[i].name;

        bool stop = true;
        for (int j = 0; j < u.size; j++) {
            stop = stop && u.covered[j];
        }
        if (stop) {
            break;
        }
    }
    min_set.size = l;
    return min_set;
}

int compare_float(const void *a, const void *b)
{
    set *fa = (set *) a;
    set *fb = (set *) b;
    return (fa->cost > fb->cost) - (fa->cost < fb->cost);
}

bool **build_cover_matrix(node_t *cares, node_t *pis, int care_num, int pi_num)
{
    bool **terms_covered = malloc(pi_num * sizeof(bool *));
    assert(terms_covered);
    for (int i = 0; i < pi_num; i++) {
        terms_covered[i] = malloc(care_num * sizeof(bool));
        assert(terms_covered[i]);
    }

    // build coverd matrix
    fprintf(stderr, "     ");
    for (node_t *a = cares; a; a = a->next) {
        print_minterm(a->val, 4, 0, stderr);
        fprintf(stderr, " ");
    }
    fprintf(stderr, "\n");

    int i, j;
    i = 0;
    for (node_t *b = pis; b; b = b->next) {
        j = 0;
        print_minterm(b->val, 4, 0, stderr);
        fprintf(stderr, " ");
        for (node_t *a = cares; a; a = a->next) {
            terms_covered[i][j] = is_covered(a->val, b->val);
            terms_covered[i][j] ? fprintf(stderr, "   X ")
                                : fprintf(stderr, "     ");
            j++;
        }
        fprintf(stderr, "\n");
        i++;
    }

    return terms_covered;
}

/*
 * test wether a is coverd by b.
 * a would not consider any don't-care variables
 */
bool is_covered(uint a, uint b)
{
    a &= MASK_EXPRESSION;
    uint exp_a = a & MASK_EXPRESSION, exp_b = b & MASK_EXPRESSION;
    uint dc_b = (b & MASK_DC_STAT) >> DC_STAT_POS;
    if (pattern_is_equal(a, b))
        return true;
    else {
        int diff_num = diff_bits(exp_a, exp_b);
        uint xored = exp_a ^ exp_b;
        for (int i = 0; i < diff_num; i++) {
            if (dc_b & 1 << (ffs(xored) - 1))
                return false;
            xored &= (xored - 1);
        }
        return true;
    }
}

int cost_minterms(node_t *head, int n)
{
    int cost = 0;
    for (node_t *iter = head; iter; iter = iter->next) {
        cost +=
            (n - count_set_bit(~((iter->val & MASK_DC_STAT) | ~MASK_DC_STAT)));
    }
    return cost;
}