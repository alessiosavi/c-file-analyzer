#include "commons.h"
int verify_binary_file(FILE *fp) {

  int n = 5;
  int ret;
  char *elf = malloc(n);
  fgets(elf, n, fp);
  // printf("\n\n\nRETRIEVED %s from FILE", elf);
  if (strcmp(elf, "ELF") == 0)
    ret = 1;
  else
    ret = 0;
  free(elf);
  fseek(fp, 0, SEEK_SET);

  return ret;
}

/* The purpouse of the methods is to return the result of "dest+source*/
char *concatenate_string(char *dest, char *source, int add_slash) {
  char *path =
      malloc(((int)strlen(dest) + (int)strlen(source) + 2) * sizeof(char));

  /*Copy the root path*/
  strcpy(path, dest);
  //  printf("\nPath after strcpy -> %s\n", path);

  /* Concatenate the dir name */
  strcat(path, source);
  if (add_slash == 1)
    strcat(path, "/");
  return path;
}

/* Return filesize -- With the best performance */
long stat_filesize(char *filename) {
  struct stat statbuf;

  if (stat(filename, &statbuf) == -1) {
    printf("failed to stat %s\n", filename);
    exit(EXIT_FAILURE);
  }
  // printf("[*] stat_filesize - file: %s, size: %ld\n", filename,
  // statbuf.st_size);
  return statbuf.st_size;
}
int stat_edit_time(const char *filename) {
  struct stat statbuf;

  if (stat(filename, &statbuf) == -1) {
    printf("failed to stat %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // printf("[*] stat_edit_time - file: %s, Date: %lu\n", filename,
  // statbuf.st_mtime);
  return statbuf.st_mtime;
}

char *read_n_data(const char *filename, int size) {

  FILE *fp;
  char *buff = (char *)malloc((size + 1) * sizeof(char));

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("error opening file :/");
    free(buff);
    return NULL;
  }

  int ret;
  ret = verify_binary_file(fp);
  if (ret == 0) {
    int byte_read;
    byte_read = fread(buff, sizeof(char), size, fp);
    buff[size] = '\0';

    if (byte_read != size) {
      printf("\n**************** ERROR %s *************\n", filename);
      free(buff);
      buff = NULL;
    }
  } else {
    printf("\n=========== SKipping binary file %s\n", filename);
    free(buff);
    buff = NULL;
  }

  fclose(fp);
  return buff;
}

/* Convert unix timestamp to a string date format using hours as delta time */
char *get_timestamp_from_unix(time_t unix_timestamp, int hours) {
  const int DATE_LEN = 27;
  struct tm ts;
  char *buf = calloc(DATE_LEN, sizeof(char));

  if (hours > 12) {
    printf(
        "\nThis software was not to work with military affiliate zonetime\n");
    exit(-1);
  }
  // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
  /* Modify time location */
  unix_timestamp += (hours * 3600);
  ts = *localtime(&unix_timestamp);
  strftime(buf, DATE_LEN, "%a %Y-%m-%d %H:%M:%S %Z", &ts);
  buf[DATE_LEN - 1] = '\0';
  return buf;
}

char *integer_to_string(int x) {
  char *buffer = malloc(sizeof(char) * (sizeof(int) * 4 + 1));
  if (buffer) {
    sprintf(buffer, "%d", x);
  }
  return buffer; // caller is expected to invoke free() on this buffer to
                 // release memory
}

/* Swap two node using their memory addresses */

void swap_node(void *a, void *b) {
  void *temp;
  temp = *(void **)a;
  *(void **)a = *(void **)b;
  *(void **)b = temp;
  // free(temp);
}

/* TIMING - BENCHMARKS */
struct timeval *get_time() {
  struct timeval *start = malloc(sizeof(struct timeval));
  gettimeofday(start, NULL);
  return start;
}

double getTimeElapsed(struct timeval end, struct timeval start) {
  return (end.tv_sec - start.tv_sec) +
         (end.tv_usec - start.tv_usec) / 1000000.00;
}

double get_time_elapsed(struct timeval *start_time) {
  struct timeval *end_time = get_time();
  double time_spent = getTimeElapsed(*end_time, *start_time);
  free(end_time);
  free(start_time);
  // printf("\nExecution time = %f\n", time_spent);
  return time_spent;
}

int ignore_binary_dir(const char *path) {
  if (strstr(path, ".git"))
    return 1;
  return 0;
}

char *tail(void *tail_input) {

  struct tail_input *t_input = (struct tail_input *)tail_input;

  const char *filename = (*t_input).filename;
  const int n_lines = (*t_input).n_lines;

  /* Will contains the character retrieved by the fgetc*/
  int c;
  /* Open the file */
  FILE *fp = NULL;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Unable to open file!\n");
    return NULL;
  }
  int new_lines_found = 0;
  /* Seeking to the end of the file */
  printf("Seeking to the end of the file\n");
  fseek(fp, 0, SEEK_END);

  /*Save last character and set file cursors position to the last character of
   * the file */
  const long last_character = ftell(fp);
  long position = last_character;

  /* Until we have found the number of new lines that we want */
  /*FIXME -> Check EOF too */
  /*TODO -> Increase performance*/
  while (new_lines_found < n_lines && position > 0 && fp != NULL) {
    if ((c = fgetc(fp)) == '\n') {
      /* Found a new line!*/
      ++new_lines_found;
    }

    fseek(fp, --position, SEEK_SET);
    // printf("reduce position at every iteration | Position = %ld\n",
    // position);
  }
  /* Line found, now we have to understand if there is at least one new line
   * before this one or if is the EOF, so iterate until found a new line or head
   * of the file */
  while (position > 0 && (c = fgetc(fp)) != '\n') {
    // printf("%ld)Searching new line ...\n", position);
    fseek(fp, --position, SEEK_SET);
  }

  /* Number of char that we want to read */
  const int char_size = last_character - position;
  char *data = malloc(char_size);

  if (fp != NULL) {
    fread(data, 1, char_size, fp);
    fclose(fp);
  }
  data[char_size - 1] = '\0';

  return data;
}
