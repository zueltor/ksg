#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 512
#define R 10  //max number of rules for 1 non terminal

typedef struct list list;
struct list {
    unsigned char c;
    list *next;
};
int count;


typedef struct nterm nterm;
struct nterm {
    char *rules[R];
    char c;
    unsigned int num_rules;
};

void insert_list(nterm *non_term, list *head, unsigned int rand_term, unsigned int rand_rule, unsigned int n,
                 unsigned int i);
list *insert_pos_0(nterm *non_term, list *head);

void get_sentence(nterm *non_term, list *head, unsigned int *n, unsigned int *found_non_term);

int kont_svob_gr(char *in, char *out, unsigned int n);
void print_help(void);