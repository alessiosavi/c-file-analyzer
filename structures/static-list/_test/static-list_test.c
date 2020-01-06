/* Unit test for the data structure */

#include "../static-list.c"

void test_01(int t, int max) {

  printf("\n========== TEST %d ==========\n", t);
  char *test_msg = "Insert element and remove root node";
  printf("\n%s %d element\n", test_msg, max);

  printf("Init %d node\n", max);
  struct list *list = init_n_node(max);
  print_nodes(list);
  printf("Node initialized");
  struct timeval *start = get_time();
  remove_node(&list, integer_to_string(0));
  get_time_elapsed(start);
  print_nodes(list);
  free_list(list);
}

void test_02(int t, int max) {

  printf("\n========== TEST %d ==========\n", t);
  char *test_msg = "Insert element and remove mid node";
  printf("\n%s %d element\n", test_msg, max);
  struct list *list = init_n_node(max);
  print_nodes(list);
  struct timeval *start = get_time();
  remove_node(&list, integer_to_string(max / 2));
  get_time_elapsed(start);
  print_nodes(list);
  free_list(list);
}

void test_03(int t, int max) {
  printf("\n========== TEST %d ==========\n", t);
  char *test_msg = "Insert element and remove last node";
  printf("\n%s %d element\n", test_msg, max);
  struct list *list = init_n_node(max);
  print_nodes(list);
  struct timeval *start = get_time();
  remove_node(&list, integer_to_string(max - 1));
  get_time_elapsed(start);
  print_nodes(list);
  free_list(list);
}
void test_04(int t, int max) {
  printf("\n========== TEST %d ==========\n", t);
  char *test_msg = "Insert element and try to remove a non existing node";
  printf("\n%s %d element\n", test_msg, max);
  struct list *list = init_n_node(max);
  print_nodes(list);
  struct timeval *start = get_time();
  remove_node(&list, integer_to_string(max));
  get_time_elapsed(start);
  print_nodes(list);
  free_list(list);
}

void test_sub_array01() {

  printf("\ntest_sub_array01 | Init int array of 4 element, then swap with "
         "counter > size");
  struct list *lista = init_list(10, "a", "a");
  add_node(&lista, "b", "b");
  add_node(&lista, "c", "c");
  add_node(&lista, "d", "d");
  print_nodes(lista);

  printf("\nSwapping array ...");
  log_file *swap =
      sub_array(lista->array, 20, lista->size, lista->max_size + 1);
  printf("\nSWAP:\n");
  print_log_file_list(swap, lista->size);
}

void test_sub_array02() {

  printf("\ntest_sub_array02 | Init int array of 4 element, then swap with "
         "counter < size");
  struct list *lista = init_list(10, "a", "a");
  add_node(&lista, "b", "b");
  add_node(&lista, "c", "c");
  add_node(&lista, "d", "d");
  int i = 0;

  print_nodes(lista);

  printf("\nSwapping array ...");
  log_file *swap = sub_array(lista->array, 20, lista->size, lista->size / 2);
  printf("\nSWAP:\n");
  print_log_file_list(swap, lista->size - 1);
}

void test_sub_array03() {

  printf("\ntest_sub_array03 | Init int array of 4 element, then swap with "
         "counter = size");
  struct list *lista = init_list(10, "a", "a");
  add_node(&lista, "b", "b");
  add_node(&lista, "c", "c");
  add_node(&lista, "d", "d");
  int i = 0;

  print_nodes(lista);

  printf("\nSwapping array ...");
  log_file *swap = sub_array(lista->array, 20, lista->size, lista->size);
  printf("\nSWAP:\n");
  print_log_file_list(swap, lista->size - 1);
}

void test_sub_array04() {

  printf("\ntest_sub_array04 | Init int array of 4 element, then swap with "
         "counter = size-1");
  struct list *lista = init_list(10, "a", "a");
  add_node(&lista, "b", "b");
  add_node(&lista, "c", "c");
  add_node(&lista, "d", "d");
  int i = 0;

  print_nodes(lista);

  printf("\nSwapping array ...");
  log_file *swap = sub_array(lista->array, 20, lista->size, lista->size - 1);
  printf("\nSWAP:\n");
  print_log_file_list(swap, lista->size - 1);
}
void test_sub_array05() {

  printf("\ntest_sub_array05 | Init int array of 4 element, then swap with "
         "counter = size-2");
  struct list *lista = init_list(10, "a", "a");
  add_node(&lista, "b", "b");
  add_node(&lista, "c", "c");
  add_node(&lista, "d", "d");
  int i = 0;

  print_nodes(lista);

  printf("\nSwapping array ...");
  log_file *swap = sub_array(lista->array, 20, lista->size, lista->size - 2);
  printf("\nSWAP:\n");
  print_log_file_list(swap, lista->size - 1);
}
void test_sub_array() {
  test_sub_array01();
  test_sub_array02();
  test_sub_array03();
  test_sub_array04();
  test_sub_array05();
}

void system_test(int test_size) {
  test_01(1, test_size);
  test_02(2, test_size);
  test_03(3, test_size);
  test_04(4, test_size);
  test_sub_array();
}

struct list *test_performance_insert(int max) {
  printf("Test performance inserting %d element", max);
  struct timeval *start = get_time();
  struct list *lista;
  int i;
  lista = init_list(10, "", "");
  for (i = 0; i < max; i++) {
    add_node(&lista, "", "");
  }
  printf("Time elapsed: %f\n", get_time_elapsed(start));
  return lista;
}

void test_performance_remove(struct list *lista, int element_index) {
  printf("Test performance deleting %d element in array of size: %d\n",
         element_index, lista->size);

  struct timeval *start = get_time();
  remove_node_by_index(&lista, element_index);
  printf("Time elapsed: %f\n", get_time_elapsed(start));
}

void test_performance(int max) { test_performance_insert(max); }

int main() {

  int test_size = 10;
  system_test(test_size);

  test_size = 1000000;
  struct list *lista = test_performance_insert(test_size);
  test_performance_remove(lista,lista->size-1);

  return 0;
}
