#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void parse_bfunc(char *, int *, node_t **, node_t **);
void get_set(char *, node_t **, int, int);
void init_hmap(node_p ****, int **, int);
void add_hmap(node_p ****, node_t *, int *, int);
void hmap_build(node_p ****, node_t *, node_t *, int);
void reduce_imps(node_p ****, node_t **, int);
void print_hmap(node_p ***, int);
int count_set_bit(int, int *);
int pattern_is_equal(uint, uint, uint);

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
    node_t *set_list, *dc_list, *imp_list;
    node_p ***hmap;  // initialize in hmap_build(...)

    init_list(set_list);
    init_list(dc_list);
    strncpy(input_filename, argv[1], 64);
    strncpy(output_filename, argv[2], 64);
    parse_bfunc(input_filename, &var_nums, &set_list, &dc_list);
    hmap_build(&hmap, set_list, dc_list, var_nums);
    print_list(dc_list);
    print_hmap(hmap, var_nums);

    return 0;
}

void parse_bfunc(char *input_file, int *var_nums, node_t **sets, node_t **dcs)
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
    get_set(buf, sets, *var_nums, MASK_SETBIT);

    fgets(buf, buf_size, fp);  //.d
    fgets(buf, buf_size, fp);
    get_set(buf, dcs, *var_nums, 0);

    free(buf);
    fclose(fp);
}

void get_set(char *str, node_t **head, int var_nums, int set_or_dc)
{
    char *token = strtok(str, " ");
    while (token != NULL) {
        uint val = atoi(token);
        val |= MASK_DCBITS;
        val |= (var_nums << VARNUM_BIT);
        val |= set_or_dc;
        add_entry(head, val);
        token = strtok(NULL, " ");
    }
}

void init_hmap(node_p ****hmap, int **pos, int n)
{
    init_list(*hmap);
    *pos = malloc(n * sizeof(int));
    (*hmap) = malloc((n + 1) * sizeof(node_p **));
    (*hmap)[0] = malloc(sizeof(node_p *));
    init_list((*hmap)[0][0]);
    for (int i = 1; i <= n; i++) {
        (*hmap)[i] = malloc(n * sizeof(node_p *));
        for (int j = 0; j < n; j++)
            init_list((*hmap)[i][j]);
        assert((*hmap)[i]);
        (*pos)[i - 1] = -1;
    }
}

void add_hmap(node_p ****hmap, node_t *list, int *pos, int n)
{
    for (node_t *iter = list; iter; iter = iter->next) {
        int g_id = count_set_bit(iter->val & MASK_VALUEBITS, pos);
        if (g_id == 0)
            add_pentry(&(*hmap)[g_id][0], &(iter->val));
        else {
            for (int i = 0; (pos[i] != -1) && i < n; i++) {
                add_pentry(&(*hmap)[g_id][pos[i]], &(iter->val));
                pos[i] = -1;
            }
        }
    }
}

void hmap_build(node_p ****hmap, node_t *sets, node_t *dcs, int var_nums)
{
    int *set_pos;
    init_hmap(hmap, &set_pos, var_nums);
    add_hmap(hmap, sets, set_pos, var_nums);
    add_hmap(hmap, dcs, set_pos, var_nums);
    free(set_pos);
}

void reduce_imps(node_p ****hmap, node_t **imp_list, int n){
    #define hmap_ref (*hmap) 
    for (node_p *current = hmap_ref[0][0]; current; current = current->next){
        for (int i=0; i<n; i++){
            for (node_p *target = hmap_ref[1][i]; target; target=target->next){
                if (pattern_is_equal(*(current->ptr), *(target->ptr), MASK_DCBITS) && !(*(current->ptr) & MASK_MERGED)){
                    /*TODO*/
                    //uint merged = merge_minterms(current->ptr, target->ptr);
                    ;
                }
            }
        }
    }
}

/*TODO*/
// uint merge_minterms(uint *a, uint *b){

// }

int pattern_is_equal(uint a, uint b, uint mask){
    return (a&mask) & (b&mask);
}

int count_set_bit(int n, int *pos)
{
    int count = 0;
    while (n) {
        pos[count] = ffs(n) - 1;
        count++;
        n &= n - 1;
    }
    return count;
}

void print_hmap(node_p ***hmap, int n)
{
    printf("hash map:\n");
    printf("\t[0][0]: ");
    print_plist(hmap[0][0]);
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < n; j++) {
            printf("\t[%d][%d]: ", i, j);
            print_plist(hmap[i][j]);
        }
    }
}