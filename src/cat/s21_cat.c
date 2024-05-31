#include "s21_cat.h"
void open_file(char *argv[], flags *flag);
int parser_flags(int argc, char *argv[], flags *flag);

int main(int argc, char *argv[]) {
  flags flag = {
      .b = 0,
      .e = 0,
      .n = 0,
      .s = 0,
      .t = 0,
      .v = 0,
  };
  parser_flags(argc, argv, &flag);
  while (optind < argc) {
    open_file(argv, &flag);
    optind++;
  }
  return 0;
}
// hhhh
const struct option long_options[] = {
    {"number-nonblank ", no_argument, 0, 'b'},
    {"number", no_argument, 0, 'n'},
    {"squeeze-blank", no_argument, 0, 's'},
    {0, 0, 0, 0},
};

int parser_flags(int argc, char *argv[], flags *flag) {
  int rez;
  while ((rez = getopt_long(argc, argv, "bEnsTvet", long_options, NULL)) !=
         -1) {
    switch (rez) {
      case 'b':
        flag->b = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      default:
        printf("usage: cat [-benstuv] [file ...]\n");
        break;
    }
  }
  return 0;
}

void open_file(char *argv[], flags *flag) {
  FILE *fp;
  if (flag->b && flag->n) flag->n = 0;
  fp = fopen(argv[optind], "r");
  // int count = 0, number = 1, last = '\n';
  if (fp != NULL) {
    int count = 0, number = 1, last = '\n';
    while (!feof(fp)) {  // чтение файла
      int ch = fgetc(fp);
      if (ch == EOF) break;
      // сжимает несколько смежных пустых строк
      if (flag->s && ch == '\n' && last == '\n') {  // s
        count++;
        if (count > 1) {
          continue;
        }
      } else {
        count = 0;
      }
      // нумерует все(не пустые) выходные строки //b & n
      if ((flag->b && last == '\n' && ch != '\n') ||
          (flag->n && last == '\n')) {
        // if (flag->b && flag->n) flag->n = 0;
        printf("%6d\t", number++);
      }
      // также отображает табы как ^I  -t предполагает и -v (-T != -v)
      if (ch == '\t' && flag->t) {  // t
        printf("^");
        ch = 'I';
      }
      //  символы конца строки как $ -e предполагает и -v (-E != -v)
      if (flag->e && ch == '\n') {  // e
        printf("$");
      }
      // показывать непечатаемые символы, кроме табуляции и конца строки. -v
      if (flag->v) {
        if (ch < 9 || (ch > 10 && ch < 32) || (ch > 126 && ch <= 160)) {
          printf("^");
          if (ch > 126) {
            ch -= 64;
          } else {
            ch += 64;
          }
        }
      }

      printf("%c", ch);
      last = ch;
    }
    fclose(fp);
  } else {
    printf("s21_cat: %s: No such file or directory\n", argv[optind]);
  }
}
