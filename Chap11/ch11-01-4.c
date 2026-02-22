#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bin_node {
  int value;
  struct bin_node *less_equal;
  struct bin_node *bigger;
} BIN_NODE;

void set(BIN_NODE *src, BIN_NODE *dest) {
  if (src->value > dest->value) {
    if (dest->bigger) {
      set(src, dest->bigger);
    } else {
      dest->bigger = src;
    }
  } else {
    if (dest->less_equal) {
      set(src, dest->less_equal);
    } else {
      dest->less_equal = src;
    }
  }
}

void print_asc(BIN_NODE *node) {
  if (node) {
    print_asc(node->less_equal);
    printf("%i ", node->value);
    print_asc(node->bigger);
  }
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "give at least 1 number");
    return 2;
  }

  BIN_NODE nodes[argc - 1];
  nodes[0].value = atoi(argv[1]);
  nodes[0].less_equal = nodes[0].bigger = NULL;

  for (int i = 2; i < argc; i++) {
    nodes[i - 1].value = atoi(argv[i]);
    nodes[i - 1].less_equal = nodes[i - 1].bigger = NULL;
    set(&nodes[i - 1], &nodes[0]);
  }

  print_asc(&nodes[0]);
}
