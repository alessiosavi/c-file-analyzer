#include "structures/static-list/static-list.c"
#include <dirent.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 10
#define LINES_MEM 1024 // 1k

void *read_tail(void *ptr_input) {
  struct tail_input *t_input = (struct tail_input *)ptr_input;
  FILE *fp;
  char *data = NULL;
  if ((fp = fopen((*t_input).filename, "r")) == NULL)
    printf("Error on file %s\n", (*t_input).filename);
  else if (verify_binary_file(fp) == 0) {
    /* Not a binary file */
    data = tail(t_input);
  } else
    printf("File %s is can't be readed, is a binary file!", (*t_input).filename);

  fclose(fp);
  return (void *)data;
}

log_file *init_node_thread(char *name, char *path) {
  /* Init node*/
  log_file *logfile = malloc(1 * sizeof(log_file));
  // Allocate the memory for copy the string + line terminator
  logfile->path = malloc(strlen(path) + 1);
  logfile->name = malloc(strlen(name) + 1);
  strcpy(logfile->name, name);
  strcpy(logfile->path, path);
  /* Number of lines to read at MAX */
  logfile->lines_number = MAX_LINES;

  /* Concatenate the string for obtain the real complete path of the file*/
  logfile->filename = concatenate_string(path, name, 0);

  /* ====  Cmon, let's threadize!! ==== */
  /* Thread for every function */
  pthread_t content_th;
  /* return value */
  void *content_ptr;

  /* Init tail input struct parameter */
  struct tail_input *t_input = calloc(1, sizeof(struct tail_input));
  (*t_input).n_lines = MAX_LINES;
  (*t_input).filename = logfile->filename;

  /* Launch the thread for tail */
  pthread_create(&content_th, NULL, read_tail, (void *)t_input);

  logfile->size = stat_filesize(logfile->filename);
  logfile->unix_timestamp = stat_edit_time(logfile->filename);
  logfile->time_last_edit = get_timestamp_from_unix(logfile->unix_timestamp, 2);

  /* Wait until thread finish */
  pthread_join(content_th, &content_ptr);
  // pthread_join(size_th, &size_ptr);
  logfile->content = (char *)content_ptr;
  free(t_input);
  return logfile;
}

/* FIXME: Use the complete_path as a double pointer in order to free at exit! */
void load_data_from_dir(char *dirname, char *complete_path,
                        struct list **list) {
  /* FIXME: Use realloc for allocate space*/
  char *path = concatenate_string(complete_path, dirname, 1);
  struct dirent *dir_struct;

  DIR *dr = opendir(path);
  if (dr != NULL) // opendir returns NULL if couldn't open directory
  {
    /* Iterating until no other files */
    while ((dir_struct = readdir(dr)) != NULL) {
      char *name = dir_struct->d_name;
      /* Avoid reference to this directory && reference to previous directory */
      if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
        /* Switch among the varius case related to the file type */
        switch (dir_struct->d_type) {

        /* File */
        case 4:
          if (ignore_binary_dir(name) == 0) {
            printf("\n========= RICORSIONE [%s] START =======\n", name);
            load_data_from_dir(name, path, list);
            printf("\n========= RICORSIONE [%s] STOP =======\n", name);
          }
          break;

        case 8:
          printf("File %s is a file!!\n", name);
          log_file *logfile = init_node_thread(name, path);
          add_log_file(list, *logfile);
          free(logfile);
          break;

        case 10:
          printf("File %s is a link!!", name);
          break;
        }
      }
    }
    closedir(dr);
    free(path);
  }
}

void recognize_modification(struct list **data) {
  int i;
  int counter = 0;
  int test = 0;
  while (test < 1000) {
    struct timeval *start = get_time();
    for (i = 0; i < (*data)->size; i++) {
      int new_time_edit = stat_edit_time((*data)->array[i].filename);
      // printf("Retrieved %d from file | %d from mem | [%s]\n",
      // new_time_edit,(*data)->array[i].unix_timestamp,
      // (*data)->array[i].filename);
      if (new_time_edit != (*data)->array[i].unix_timestamp) {
        printf("Data of file %s changed!\n", (*data)->array[i].filename);
        (*data)->array[i] =
            *init_node_thread((*data)->array[i].name, (*data)->array[i].path);
      }
    }
    double time_elapsed = get_time_elapsed(start);
    printf("%d) Time: %f\n", ++counter, time_elapsed);
    // sleep(5);
    ++test;
  }
}

int main(void) {
  /* Initialize a new list*/
  struct list *list = calloc(1, sizeof(struct list));

  /* Read every file for every subdirectory */
  load_data_from_dir(".", "", &list);
  /* Iterate every file and print the information */
  print_log_file_list(list->array, list->size);

  recognize_modification(&list);

  free_list(list);
  // free(list);

  printf("\n");
  return 0;
}
