#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../structures/datastructure.h"
int verify_binary_file(FILE *fp);
char *concatenate_string(char *dest, char *source, int add_slash) ;
long stat_filesize(char *filename);
int stat_edit_time(const char *filename);
char *read_n_data(const char *filename, int size);
char *get_timestamp_from_unix(time_t unix_timestamp, int hours);
char *integer_to_string(int x);
void swap_node(void *a, void *b);
struct timeval *get_time();
double getTimeElapsed(struct timeval end, struct timeval start);
double get_time_elapsed(struct timeval *start_time);
int ignore_binary_dir(const char *path);
char *tail(void *tail_input);
