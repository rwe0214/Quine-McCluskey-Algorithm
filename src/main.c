#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void parse_bfunc(char *, int *, node_t **, node_t **);
void get_set(char *, node_t **, int);
void init_hmap(node_t ***, int);
void add_hmap(node_t ***, node_t *, int);
void hmap_build(node_t ***, node_t *, node_t *, int);

void print_hmap(node_t **, int);
int count_set_bit(int);
int is_mergeable(uint a, uint b);
int diff_bits(uint, uint);
int pattern_is_equal(uint, uint);
void free_map(node_t ***, int);

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: ./hw1.o [input_file_name] [output_file_name]\n\n"
                "Quine Mccluskey Algorithm\n\n"
                "Arguments:\n"
                "\tinput_file_name\n"
                "\toutput_file_name\n");
        return 1;
    }

    char input_filename[64];
    char output_filename[64];
    int var_nums;
    node_t *cares, *dont_cares, *minterms;

    node_t **hmap[10];

    init_list(cares);
    init_list(dont_cares);
    init_list(minterms);
    for (int i=0; i<10; i++)
        init_list(hmap[i]);
    strncpy(input_filename, argv[1], 64);
    strncpy(output_filename, argv[2], 64);

    parse_bfunc(input_filename, &var_nums, &cares, &dont_cares);
    hmap_build(&hmap[0], cares, dont_cares, var_nums);
    print_hmap(hmap[0], var_nums);

    // TODO: Reduce implicants
    // for(int i=0; i<10; i++){
    //     node_t **next_col;
    //     init_list(next_col);
    //     for(int j=0, k=1; j<var_nums; j++, k=j+1){
    //         hmap[i][j]
    //     }
    // }


    free_list(cares);
    free_list(dont_cares);
    free_map(hmap, var_nums);
    return 0;
}

void parse_bfunc(char *input_file,
                 int *var_nums,
                 node_t **cares,
                 node_t **dont_cares)
{
    FILE *fp = fopen(input_file, "r");
    assert(fp);
    int buf_size = 256;
    char *buf = (char *) malloc(buf_size * sizeof(char));
    assert(buf);

    fgets(buf, buf_size, fp);
    sscanf(buf, ".i %d", var_nums);
    fgets(buf, buf_size, fp);  //.m
    fgets(buf, buf_size,
          fp);  // Here would be an issue if there exist empty on-set
    get_set(buf, cares, *var_nums);

    fgets(buf, buf_size, fp);  //.d
    fgets(buf, buf_size, fp);
    get_set(buf, dont_cares, *var_nums);

    free(buf);
    fclose(fp);
}

void get_set(char *str, node_t **head, int var_nums)
{
    char *token = strtok(str, " ");
    while (token != NULL) {
        uint val = atoi(token);
        val |= MASK_DC_STAT;
        add_entry(head, val);
        token = strtok(NULL, " ");
    }
}

void init_hmap(node_t ***hmap, int n)
{
    *hmap = malloc((n + 1) * sizeof(node_t *));
    assert(*hmap);
    for (int i = 0; i <= n; i++) {
        init_list((*hmap)[i]);
    }
}

void add_hmap(node_t ***hmap, node_t *list, int n)
{
    for (node_t *iter = list; iter; iter = iter->next) {
        int h_id = count_set_bit(iter->val & MASK_EXPRESSION);
        add_entry(&(*hmap)[h_id], iter->val);
    }
}

void hmap_build(node_t ***hmap, node_t *sets, node_t *dcs, int var_nums)
{
    init_hmap(hmap, var_nums);
    add_hmap(hmap, sets, var_nums);
    add_hmap(hmap, dcs, var_nums);
}

int is_mergeable(uint a, uint b)
{
    if (diff_bits(a & MASK_EXPRESSION, b & MASK_EXPRESSION) == 1 &&
        pattern_is_equal(a & MASK_DC_STAT, b & MASK_DC_STAT))
        return 1;
    return 0;
}

int diff_bits(uint a, uint b)
{
    return count_set_bit(a ^ b);
}

int pattern_is_equal(uint a, uint b)
{
    return diff_bits(a, b) ? 0 : 1;
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

void print_hmap(node_t **hmap, int n)
{
    printf("hash map:\n");
    for (int i = 0; i <= n; i++) {
        printf("\t[%d]: ", i);
        print_list(hmap[i]);
    }
}

void free_map(node_t ***map, int n){
    for(int i=0; i<n; i++){
        if(!map[i])
            continue;
        for(int j=0; j<n; j++)
            free_list(map[i][j]);
        free(map[i]);
    }
}