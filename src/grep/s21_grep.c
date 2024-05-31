#include "s21_grep.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc > 2) parthing_input(argc, argv);
  return 0;
}

void parthing_input(int argc, char **argv) {
  struct gflags flags = {0};
  parthing_flags(argc, &flags, argv);
  if (flags.e == 0 && flags.f == 0 && flags.pattern_count == 0) {
    for (int i = 1; i < argc - 1; i++) {
      if (argv[i][0] != '-') {
        flags.e = 1;
        save_pattern_e(&flags, argv[i]);
        flags.pattern_position = i;
        break;
      }
    }
  }
  processing(argc, &flags, argv);
}

void parthing_flags(int argc, struct gflags *flags, char **argv) {
  for (int i = 1; i < argc; i++)
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        switch (argv[i][j]) {
          case 'e':
            flags->e++;
            save_pattern_e(flags, argv[i + 1]);
            break;

          case 'i':
            flags->i = 1;
            break;

          case 'v':
            flags->v = 1;
            break;

          case 'c':
            flags->c = 1;
            break;

          case 'l':
            flags->l = 1;
            break;

          case 'n':
            flags->n = 1;
            break;

          case 'h':
            flags->h = 1;
            break;

          case 's':
            flags->s = 1;
            break;

          case 'f':
            flags->f++;
            save_pattern_f(flags, argv[i + 1]);
            flags->pattern_position = i + 1;
            break;

          case 'o':
            flags->o = 1;
            break;

          default:
            flags->error = 1;
        }
      }
    }
}

void save_pattern_f(struct gflags *flags, const char *argv) {
  if (flags->f > 0) {
    FILE *file = NULL;
    file = fopen(argv, "r");
    if (file != NULL) {
      int ch = 0;
      flags->pattern_count = strlen(flags->pattern);
      while (ch != EOF) {
        ch = fgetc(file);
        if (ch == '\n' || ch == '\0') ch = '|';
        flags->pattern[flags->pattern_count] = ch;
        flags->pattern_count++;
      }
      if (fclose(file) != 0)
        fprintf(stderr, "s21_grep: %s: file close error", argv);
    }
  }
}

void save_pattern_e(struct gflags *flags, const char *argv) {
  if (flags->e > 0) {
    strcat(flags->pattern, argv);
    strcat(flags->pattern, "|");
  }
}

void processing(int argc, struct gflags *flags, char **argv) {
  FILE *file = NULL;
  regex_t exp;
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int i = 0;
  flags->pattern[strlen(flags->pattern) - 1] = '\0';
  flags->pattern[strlen(flags->pattern)] = '\0';
  if (flags->i == 1) regcomp(&exp, flags->pattern, REG_ICASE | REG_EXTENDED);
  if (flags->i == 0) regcomp(&exp, flags->pattern, REG_EXTENDED);
  for (i = 1; i < argc; i++) {
    if (argv[i][0] != '-' && i != flags->pattern_position &&
        ((argv[i - 1][0] == '-' && argv[i - 1][1] != 'e' &&
          argv[i - 1][1] != 'f') ||
         (argv[i - 1][0] != '-'))) {
      flags->fcount++;
      // printf("\n fcount = %d\n", flags->fcount);
    }
  }
  for (i = 1; i < argc; i++) {
    if (argv[i][0] != '-' && i != flags->pattern_position &&
        ((argv[i - 1][0] == '-' && argv[i - 1][1] != 'e' &&
          argv[i - 1][1] != 'f') ||
         (argv[i - 1][0] != '-'))) {
      file = fopen(argv[i], "r");
      if (file != NULL) {
        flags->linecount = 0;
        flags->ccount = 0;
        while (fgets(flags->str, 8192, file)) {
          flags->linecount++;
          if (flags->v == 1 &&
              regexec(&exp, flags->str, nmatch, pmatch, 0) == 1) {
            flags->ccount++;
            if (flags->l == 1) {
              flags->l++;
              break;
            }
            if (flags->c == 0) {
              output(flags, argv, i, nmatch, pmatch, exp);
            }

          } else if (flags->v == 0 &&
                     regexec(&exp, flags->str, nmatch, pmatch, 0) == 0) {
            flags->ccount++;
            if (flags->l == 1) {
              flags->l++;
              break;
            }
            if (flags->c == 0) {
              output(flags, argv, i, nmatch, pmatch, exp);
            }
          }
        }
        if (flags->c == 1) {
          if (flags->h == 0 && flags->fcount > 1) printf("%s:", argv[i]);
          printf("%d\n", flags->ccount);
        }
        if (flags->l > 1) {
          printf("%s\n", argv[i]);
          flags->l = 1;
        }
        if (fclose(file) != 0)
          fprintf(stderr, "s21_grep: %s: file close error", argv[i]);

      } else {
        if (flags->s == 0)
          fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                  argv[i]);
      }
    }
  }
  regfree(&exp);
}

void output(struct gflags *flags, char **argv, int i, size_t nmatch,
            regmatch_t pmatch[1], regex_t exp) {
  if (flags->h == 0 && flags->fcount > 1) printf("%s:", argv[i]);
  if (flags->n == 1) printf("%d:", flags->linecount);
  if (flags->o == 1 && flags->v == 0) {
    int eflag = 0;
    char *sstr = flags->str;
    while (regexec(&exp, sstr, nmatch, pmatch, eflag) == 0) {
      for (int o = pmatch[0].rm_so; o < pmatch[0].rm_eo; o++)
        printf("%c", sstr[o]);
      printf("\n");
      sstr += pmatch[0].rm_eo;
      eflag = REG_NOTBOL;
    }
  } else {
    printf("%s", flags->str);
  }
}
