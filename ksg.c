#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

void insert_list(nterm *non_term, list *head, unsigned int rand_term, unsigned int rand_rule, unsigned int n,
                 unsigned int i) {
    head->c = non_term[rand_term].rules[rand_rule][i++];
    head->next = (list *) malloc(sizeof(list));
    if (i == n)
        head->next = NULL;
    else
        insert_list(non_term, head->next, rand_term, rand_rule, n, i);
}

list *insert_pos_to_0(nterm *non_term, list *head) {
    list *new_head = (list *) malloc(sizeof(list));
    for (int i = 0; i < 26; i++) {
        if (non_term[i].c == head->c) {
            int rand_rule = rand() % (non_term[i].num_rules);
            insert_list(non_term, new_head, i, rand_rule, strlen(non_term[i].rules[rand_rule]), 0);
            break;
        }
    }
    list *p_last_sym = new_head;
    while (1) {
        if (p_last_sym->next == NULL) {
            break;
        } else p_last_sym = p_last_sym->next;
    }
    p_last_sym->next = head->next;
    return new_head;
}


void get_sentence(nterm *non_term, list *head, unsigned int *n, unsigned int *found_non_term) {
    if (count > *n)
        return;
    if (*found_non_term == 2)
        return;
    if ((head == NULL) && (*found_non_term == 0)) {
        *found_non_term = 2;
        return;
    }
    if (head->c < 'A' || head->c > 'Z') {
        get_sentence(non_term, head->next, n, found_non_term);
    } else {
        *found_non_term = 1;
        return;
    }
    while (*found_non_term == 1) {
        list *tmp = (list *) malloc(sizeof(list));
        for (int i = 0; i < 26; i++) {
            if (non_term[i].c == head->next->c) {
                int rand_rule = rand() % (non_term[i].num_rules);
                insert_list(non_term, tmp, i, rand_rule, strlen(non_term[i].rules[rand_rule]), 0);
                break;
            }
        }
        list *p_last_sym = tmp;
        while (p_last_sym->next != NULL)
            p_last_sym = p_last_sym->next;

        p_last_sym->next = head->next->next;
        head->next = tmp;
        count++;
        *found_non_term = 0;
        get_sentence(non_term, head->next, n, found_non_term);
    }
    if (*found_non_term == 2)
        return;
}

int kont_svob_gr(char *in, char *out, unsigned int n) {
    unsigned int i,
            j,
            m;
    unsigned char buf[N];
    FILE *f1 = fopen(in, "rb");
    FILE *f2 = fopen(out, "wb");

    if (f1 == NULL) {
        printf("error %s", in);
        return 1;
    }
    if (f2 == NULL) {
        printf("error %s", out);
        return 1;
    }

    j = 0;
    m = 0;
    nterm non_term[26];
    int state = 1;
    i = 0;

    while (1) {
        if (state == 1) {
            if (fgets(buf, N, f1) == NULL)
                break;
            non_term[i].c = buf[0];
            state = 2;
        }
        if (state == 2) {
            fgets(buf, N, f1);
            if (buf[0] == ';') {
                state = 1;
                non_term[i].num_rules = j;
                j = 0;
                i++;
            } else {
                m = strlen(buf) - 3;
                non_term[i].rules[j] = (char *) malloc(m * sizeof(char));
                memset(non_term[i].rules[j], 0, m * sizeof(char));
                strncpy(non_term[i].rules[j], buf + 2, m - 1);
                j++;
            }
        }
    }
    list *head = (list *) malloc(sizeof(list));

    srand(time(NULL));

    unsigned int rand_nterm = rand() % i;
    unsigned int rand_rule = rand() % (non_term[rand_nterm].num_rules);
    m = strlen(non_term[rand_nterm].rules[rand_rule]);

    //initialising expression using random non terminal symbol and random rule
    insert_list(non_term, head, rand_nterm, rand_rule, strlen(non_term[rand_nterm].rules[rand_rule]), 0);
    list *tmp = head;

    printf("initial expression:\n");
    while (tmp != NULL) {
        printf("%c", tmp->c);
        tmp = tmp->next;
    }
    printf("\n");

    m = 0;
    j = 0;
    count = 0;

    while (head->c >= 'A' && head->c <= 'Z') {
        head = insert_pos_to_0(non_term, head);
        count++;
        if (count > n)
            break;
    }

    if (count <= n) {
        get_sentence(non_term, head, &n, &m);
    }

    printf("number of substitutions = %d\n", count);
    while (head != NULL) {
        printf("%c", head->c);
        head = head->next;
    }
    if (count > n)
        printf("\ncount exceeded n");
    printf("\n");
    fclose(f1);
    fclose(f2);
    return 0;
}

void print_help(void) {
    printf("Usage: <input_file_name> <output_file_name> <N>");
}