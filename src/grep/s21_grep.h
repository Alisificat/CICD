#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <regex.h>
#include <stdlib.h>

struct gflags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int error;
  char pattern[8192];
  int pattern_position;
  int pattern_count;
  char str[8192];
  int ccount;
  int fcount;
  int linecount;
};

void parthing_input(int argc, char **argv);
void parthing_flags(int argc, struct gflags *flags, char **argv);
void save_pattern_e(struct gflags *flags, const char *argv);
void save_pattern_f(struct gflags *flags, const char *argv);
void processing(int argc, struct gflags *flags, char **argv);
void output(struct gflags *flags, char **argv, int i, size_t nmatch,
            regmatch_t pmatch[1], regex_t exp);
#endif  // SRC_GREP_S21_GREP_H_
