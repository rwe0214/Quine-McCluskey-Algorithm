#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists.h"
#include "mccluskey.h"

void parse_bfunc(char *, int *, node_t **, node_t **);
void get_set(char *, node_t **, int);


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
    for (int i = 0; i < 10; i++)
        init_list(hmap[i]);
    strncpy(input_filename, argv[1], 64);
    strncpy(output_filename, argv[2], 64);

    parse_bfunc(input_filename, &var_nums, &cares, &dont_cares);
    hmap_build(&hmap[0], cares, dont_cares, var_nums);

    run_mccluskey(hmap, var_nums);
    print_hmap(hmap, var_nums);

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

