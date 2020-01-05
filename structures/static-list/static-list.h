#ifndef _STATI_LIST_H_
#define _STATI_LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include "../../utils/commons.c"
#include "../datastructure.h"

log_file init_node(char *name, char *path);
void print_log_file_list(log_file *logfiles, int size);
void print_nodes(struct list *data);

log_file *sub_array(log_file *B, int new_size, int size_used, int counter);
log_file create_log_file(char *name, char *path, int size, char *content, int line_to_save, int lines_number);
struct list *init_log_file_list(int size, log_file logfile);
struct list *init_list(int size, char *name, char *path);
void add_log_file(struct list **data, log_file logfile);
void add_node(struct list **data, char *name, char *path);
void remove_node(struct list **data, char *name);
struct list *init_n_node(int n);
void free_list(struct list *list);

#endif
