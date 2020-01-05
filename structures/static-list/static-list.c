#include "static-list.h"
/* Enable logging */
// #define DEBUG 1
#define MIN_LIST_LENGHT 20
#define DEFAULT_INC MIN_LIST_LENGHT

log_file init_node(char *name, char *path) {
  log_file *logfile = calloc(1, sizeof(log_file));
  (*logfile).name = malloc(strlen(name) + 1);
  (*logfile).path = malloc(strlen(path) + 1);
  strcpy((*logfile).name, name);
  strcpy((*logfile).path, path);
  return *logfile;
}

void print_log_file_list(log_file *logfiles, int size) {
  if (logfiles == NULL) {
    printf("\nprint_int_array | ARRAY DATA not initialized");
    return;
  }
  int i;
  for (i = 0; i < size; i++) {
    printf("\nFilename: %s | ", logfiles[i].filename);
    printf("Name: %s | ", logfiles[i].name);
    printf("Path: %s | ", logfiles[i].path);
    printf("Size: %ld | ", logfiles[i].size);
    printf("Number of lines: %d | ", logfiles[i].lines_number);
    printf("Timestamp: %d | ", logfiles[i].unix_timestamp);
    printf("Last Edit: |%s| ", logfiles[i].time_last_edit);
    printf("Content:\n%s| Content-stop ", logfiles[i].content);
  }
}

void print_nodes(struct list *data) {
  if (data == NULL)
    printf("\nprint_nodes | Data null!!");
  else
    print_log_file_list(data->array, data->size);
  printf("\n");
}
/* Shift the value of the array and allocate a new one of a given size
 * The method is delegated to delete the element of index 'counter' shifting the
 * content of the array. The portion of the array since 'counter' will be copied
 * into
 * the new allocated array; then the other part of the data (starting from
 * 'counter+1) will be copied.
 * */

log_file *sub_array(log_file *B, int new_size, int size_used, int counter) {
  /* Be sure that the value that we want to shift is enclosed in the array size
   */
  if (counter > size_used || counter < 0) {
    /*TODO: Print a message for every error cause */
    printf("\nsub_array | Invalid index!");
    return B;
  }
  /* This array will contains the new data */
  log_file *new_list = malloc(new_size * sizeof(log_file));
  /* Copy the size of (n element before * int size) from the input array
   * starting from first element [*array -> array[0]]*/
  printf("\nsub_array | COPYING FIRST %d elements ...", counter);
  memmove(new_list, B, (counter) * sizeof(log_file));
#ifdef DEBUG
  printf("\nNew list data after FIRST memcopy-> ");
  print_log_file_list(new_list, new_size);
#endif
  /* Copy everything starting from the end of the size that we have copied
   * before from counter until the last element that the array contains */
  printf("\nCopying %d elements starting from %d", (size_used - counter - 1),
         counter + 1);
  memmove(&new_list[counter], &B[counter + 1],
          (new_size - counter - 1) * sizeof(log_file));
#ifdef DEBUG
  printf("\nNew list data after SECOND memcopy-> ");
  print_log_file_list(new_list, new_size);
#endif
  /* We have deleted a value, so we decrease the number of element*/
  --size_used;
  if (counter == new_size)
    printf("\nsub_array | Not necessary to initialize memory, array full!");
  else {
    printf("\nInitializing with -1 from %d for %d elements", size_used - 1,
           (new_size - size_used));
    /* Setting -1 starting from the last element of the list till the end */
    printf("\nInitializing: %d", new_size - size_used);
    memset(&new_list[size_used], 0, (new_size - size_used) * sizeof(log_file));
  }
#ifdef DEBUG
  printf("\nNew list data after memset> ");
  print_log_file_list(new_list, new_size);
#endif
  return new_list;
}

log_file create_log_file(char *name, char *path, int size, char *content,
                         int line_to_save, int lines_number) {
  log_file log_file = init_node(name, path);
  log_file.size = size;
  log_file.content = content;
  log_file.lines_to_save = line_to_save;
  log_file.filename = concatenate_string(path, name, 1);
  log_file.time_last_edit =
      get_timestamp_from_unix(stat_edit_time(log_file.filename), 2);
  log_file.lines_number = lines_number;
  return log_file;
}

/* Initalize the structure that contains the data */
struct list *init_log_file_list(int size, log_file logfile) {
  /* FORCE to MIN_LIST_LENGHT */
  if (size <= 0)
    size = MIN_LIST_LENGHT;
  /* Creating uninitialized memory for the struct */
  struct list *list = calloc(1, sizeof(struct list));
  /* Be sure that the memory was really allocated */
  if (!list) {
    printf("\nOUT OF MEMORY!!");
    return NULL;
  }
  /* This will be the array that will contains the data */
  list->array = calloc(size, sizeof(log_file));
  if (!list->array) {
    printf("\nOUT OF MEMORY!!");
    free(list);
    // FIXME: Handle null
    return NULL;
  }
  /* the size rappresent the number of node inserted in the list (only the root
   * for now) */
  /* The node just add, will increase when the value will be set few instruction
   * below */
  list->size = 0;
  list->max_size = size; /* Parameter used for allocate the space */
  /* array that contains the value just inserted */
  /* Assign to 0 and increase size*/
  list->array[list->size++] = logfile;
  return list;
}

/* Initalize the structure that contains the data */
struct list *init_list(int size, char *name, char *path) {
  /* FORCE to MIN_LIST_LENGHT */
  if (size <= 0) {
    size = MIN_LIST_LENGHT;
  }

  /* Creating uninitialized memory for the struct */
  struct list *list = calloc(1, sizeof(struct list));
  /* Be sure that the memory was really allocated */
  if (!list) {
    printf("\nOUT OF MEMORY!!");
    return NULL;
  }
  /* This will be the array that will contains the data */
  list->array = calloc(size, sizeof(log_file));
  if (!list->array) {
    printf("\nOUT OF MEMORY!!");
    free(list);
    // FIXME: Handle null
    return NULL;
  }
  /* the size rappresent the number of node inserted in the list (only the root
   * for now) */
  /* The node just add, will increase when the value will be set few instruction
   * below */
  list->size = 0;
  list->max_size = size; /* Parameter used for allocate the space */
  /* array that contains the value just inserted */
  list->array[list->size++] = init_node(name, path);
  return list;
}

/* Use the address of the struct for add a node the list */
void add_log_file(struct list **data, log_file logfile) {
  /* List is not initialized, init the first node */

  if (*data != NULL) {

    if /* check if is necessary to allocate a new element */
        ((*data)->size == (*data)->max_size) {
      /* List have reached the maximum space! Allocate some new fresh memory */
      /* Access and save the old size */
      int old_max_size = (*data)->max_size;
      /* Increase the size by a constant factor */
      int new_size = old_max_size + DEFAULT_INC;
      //   printf("\nadd_node | Allocate a new space for store %d", value);
      /*Allocate a new space for the data */
      (*data)->array = realloc((*data)->array, new_size * sizeof(log_file));
      /* Set the new max size*/
      (*data)->max_size = new_size;
    }

    /* Add the element to the new list and use the post increment to increase
     * the size*/
    (*data)->array[(*data)->size++] = logfile;
  } else {
    printf("\nadd_node | INIT ROOT NODE %s\n", logfile.name);
    *data = init_log_file_list(MIN_LIST_LENGHT, logfile);
  }
}

/* Use the address of the struct for add a node the list */
void add_node(struct list **data, char *name, char *path) {
  /* List is not initialized, init the first node */
  if (*data == NULL) {
    printf("\nadd_node | INIT ROOT NODE %s\n", name);
    *data = init_list(MIN_LIST_LENGHT, name, path);
    return;
  } else if /* check if is necessary to allocate a new element */
      ((*data)->size == (*data)->max_size) {
    /* List have reached the maximum space! Allocate some new fresh memory */
    /* Access and save the old size */
    int old_max_size = (*data)->max_size;
    /* Increase the size by a constant factor */
    int new_size = old_max_size + DEFAULT_INC;
    //   printf("\nadd_node | Allocate a new space for store %d", value);

    /*Allocate a new space for the data */
    (*data)->array = realloc((*data)->array, new_size * sizeof(log_file));
    /* Set the new max size*/
    (*data)->max_size = new_size;
  }
  /* Add the element to the new list and use the post increment to increase
   * the size*/
  (*data)->array[(*data)->size++] = init_node(name, path);
}

void remove_node(struct list **data, char *name) {
  /* Be sure that the list and the array are (at least) allocated */
  if (*data == NULL || (*data)->array == NULL) {
    printf("\nremove_node | LIST EMPTY!");
    return;
  }
  int counter = 0;
  /* Iterating the data ... */
  while (counter < (*data)->size) {
    /* Data found! */
    if (strcmp((*data)->array[counter].name, name) == 0) {
      /* Delete the address copying the memory before and after the counter */
      const int new_size = (*data)->max_size;
      /* This will contains the new data */
      log_file *new_list =
          sub_array((*data)->array, new_size, (*data)->size, counter);
      /* Free the data */
      free((*data)->array);
      /* Copy the pointer to the data*/
      (*data)->array = new_list;
      /* Set the new max size*/
      (*data)->max_size = new_size;
      /* Decrease counter of the size */
      --(*data)->size;
      printf("\nremove_node | Value [%s] | POS: [%d] | Removed! ", name,
             counter);
      return;
    }
    ++counter;
  }
  printf("\nRemove_node | Data [%s] not found ... ", name);
}

struct list *init_n_node(int n) {
  struct list *list = NULL;
  int i;
  for (i = 0; i < n; i++) {
    add_node(&list, integer_to_string(i), integer_to_string(i * i));
  }
  return list;
}

void free_list(struct list *list) {
  int i = 0;
  for (; i < list->size; i++) {

    if (list->array[i].name != NULL) {
#ifdef DEBUG
      printf("\nFree Name: %s\n", list->array[i].name);
#endif
      free(list->array[i].name);
    }
    if (list->array[i].path != NULL) {
#ifdef DEBUG
      printf("\nFree Path: %s\n", list->array[i].path);
#endif
      free(list->array[i].path);
    }

    if (list->array[i].filename != NULL) {
#ifdef DEBUG
      printf("\nFree Filename\n");
#endif
      free(list->array[i].filename);
    }
    if (list->array[i].time_last_edit != NULL) {
#ifdef DEBUG
      printf("\nFree Time\n");
#endif
      free(list->array[i].time_last_edit);
    }
    if (list->array[i].content != NULL) {
#ifdef DEBUG
      printf("\nFree Content\n");
#endif
      free(list->array[i].content);
    }
  }
  free(list->array);
  free(list);
}
