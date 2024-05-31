#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flags;

void open_file(char *argv[], flags *flag);

#endif  // SRC_CAT_S21_CAT_H_
