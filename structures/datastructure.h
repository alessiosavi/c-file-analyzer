#ifndef _COMMON_DATASTRUCTURES_
#define _COMMON_DATASTRUCTURES_

#include <time.h>

/* Used as log file */
typedef struct log_file {
  /* Name of the log file */
  char *name;
  /* Absolute path */
  char *path;
  /* Concatenation of name and path */
  char *filename;
  /* Used for check change in the file */
  long size;
  /* Used for save the number of lines */
  int lines_number;
  /* Timestamp of last modification */
  char *time_last_edit;

  /* Unix timestamp of last edit */
  int unix_timestamp; 
  /* Content of the file */
  char *content;
  /* Number of lines to read */
  short lines_to_save;

}log_file;


/* Input parameter for custom tail function */
typedef struct tail_input {
  /* Filename that have to be read*/
 char *filename;
  /* Number of lines that have to be read*/
int n_lines;
} tail_input;


/*
 * Static list C implementation
 *
 * This datastructure contains an array that store the value that we want to save;
 * The array have a fixed size, once the array is full and the user try to insert a new 
 * node, the size will grow of DEFAULT_INC
 *
 * Structure
 * - int *array:
 *   the array contains the value that we want to store
 * - int size:
 *   number of value that the array contains
 * - int max_size:
 *   max number of value that the array can contains before allocate a new array
 * 
 * */
struct list {
  /* This array will be a static array of size == max_size */
  log_file *array;
  int size;
  int max_size;
};
#endif
