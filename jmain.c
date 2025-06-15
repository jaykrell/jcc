/* Combine exes into one for faster (clean) build. Link once. */

#include "jmain.h"
#include "jmax.h"
#include <stdio.h>
#include <string.h>

void remove_exe(char *exe) {
  size_t len = strlen(exe);
  if (len <= 4)
    return;
  exe += len - 4;
  if (strcmp(exe, ".exe"))
    return;
  *exe = 0;
}

int main(int argc, char **argv) {
  char *arg0 = argv[0];
  char *arg1 = strrchr(arg0, '\\');
  char *arg2 = strrchr(arg0, '/');
  argc;
  arg0 = JMAX(arg1, arg2);
  if (arg0)
    ++arg0;
  else
    arg0 = argv[0];
  remove_exe(arg0);
  if (!strcmp(arg0, "test_hash"))
    return test_hash();
  if (!strcmp(arg0, "test_list"))
    return test_list();
  if (!strcmp(arg0, "test_os"))
    return test_os();
  if (!strcmp(arg0, "test_thread"))
    return test_thread();
  if (!strcmp(arg0, "test_varint"))
    return test_varint();
  if (!strcmp(arg0, "test_dec"))
    return test_dec();
  if (!strcmp(arg0, "test_vec"))
    return test_vec();
  if (!strcmp(arg0, "test_vec1"))
    return test_vec1();
  if (!strcmp(arg0, "csv1"))
    return csv1(argc, argv);

  fprintf(stderr, "ERROR: unknown argv[0]:%s\n", arg0);
  return 1;
}
