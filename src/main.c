#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

node_t **parse_bfunc(char *,
                     int *);  // Return on-set and dc-set as an array of list
node_t *get_set(char *);
int count_set_bit(int);

int main(int argc, char **argv)
{
    node_t **lists_set;
    int var_num;
    lists_set = parse_bfunc("sample_input", &var_num);
    print_list(lists_set[0]);
    print_list(lists_set[1]);

    for (int i = 0; i < 2; i++)
        free_list(lists_set[i]);

    return 0;
}

node_t **parse_bfunc(char *input_file, int *var_nums)
{
    FILE *fp = fopen(input_file, "r");
    assert(fp);
    int buf_size = 128;
    char *buf = (char *) malloc(buf_size * sizeof(char));
    assert(buf);

    fgets(buf, buf_size, fp);
    sscanf(buf, ".i %d", var_nums);
    fgets(buf, buf_size, fp);  //.m
    fgets(buf, buf_size,
          fp);  // Here would be an issue if there exist empty on-set
    node_t **lists_set = malloc(2 * sizeof(node_t *));
    assert(lists_set);
    lists_set[0] = get_set(buf);
    fgets(buf, buf_size, fp);  //.d
    fgets(buf, buf_size, fp);
    lists_set[1] = get_set(buf);

    free(buf);
    return lists_set;
}

node_t *get_set(char *str)
{
    char *token = strtok(str, " ");
    node_t *head = NULL;
    while (token != NULL) {
        add_entry(&head, atoi(token));
        token = strtok(NULL, " ");
    }
    return head;
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
