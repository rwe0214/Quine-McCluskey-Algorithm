#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists.h"
#include "mccluskey.h"

void init(char **);
void parse_bfunc(char *, int *, node_t **, node_t **);
void get_set(char *, node_t **, int);
void write_minterms(char *, node_t *, char *, char *);
void free_mem();

char input_filename[64];
char output_filename[64];
int var_nums, care_num, pi_num;
node_t *cares, *dont_cares, *prime_implicants, *min_covers;
node_t **hmap[10];

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

    init(argv);
    parse_bfunc(input_filename, &var_nums, &cares, &dont_cares);
    build_hmap_col(&hmap[0], cares, dont_cares, var_nums);
    run_mccluskey(hmap, var_nums, &prime_implicants);
    write_minterms(output_filename, prime_implicants, ".pi", "w");
    find_minimal_cover(&min_covers, &prime_implicants, &cares, var_nums);
    write_minterms(output_filename, min_covers, ".mc", "a");
    free_mem();

    return 0;
}

void init(char **argv)
{
    init_list(cares);
    init_list(dont_cares);
    init_list(prime_implicants);
    init_list(min_covers);
    for (int i = 0; i < 10; i++)
        init_list(hmap[i]);
    strncpy(input_filename, argv[1], 64);
    strncpy(output_filename, argv[2], 64);
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
    buf = NULL;
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

void write_minterms(char *output_file,
                    node_t *minterms,
                    char *prefix,
                    char *mode)
{
    FILE *fp = fopen(output_file, mode);
    assert(fp);
    int buf_size = 256;
    char *buf = (char *) malloc(buf_size * sizeof(char));
    assert(buf);

    fprintf(fp, "%s %d\n", prefix, sizeof_list(minterms));
    print_list_minterm(minterms, var_nums, 0, '\n', fp);
    if (prefix[1] == 'm' && prefix[2] == 'c')
        fprintf(fp, "cost=%d", cost_minterms(minterms, var_nums));
    else
        fprintf(fp, "\n");
    free(buf);
    fclose(fp);
}

void free_mem()
{
    free_list(cares);
    free_list(dont_cares);
    free_list(min_covers);
    free_list(prime_implicants);
    free_map(hmap, var_nums);
}
