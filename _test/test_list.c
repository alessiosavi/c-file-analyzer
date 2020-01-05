#include "../structures/static-list/static-list.c"
/* Used as list */

int main() {

  char *name = malloc(strlen("alessio")+1);
  char *path = malloc(strlen("savi")+1);
  strcpy(name,"alessio");
  strcpy(path,"savi");

  struct list *list = init_list(1, name, path);



  free(name);
  free(path);

  add_node(&list, "alessio1", "savi1");
  add_node(&list, "alessio2", "savi2");

  print_nodes(list);
  printf("\n==================\n");
  remove_node(&list, "alessio2");
  printf("\n==================\n");

  print_nodes(list);

  //free(list);
  free_list(list);

  return 0;
}
