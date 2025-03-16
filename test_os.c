/* test_os.c */

#pragma warning(disable : 4100) /* unused parameter */
#include "jos.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  /*
  int jos_open_file_read(const char*, int* file_handle);
  int jos_open_file_write(const char*, int* file_handle);
  int jos_set_file_size(int file_handle, int64_t file_size);;
  int jos_get_file_size(int file_handle, int64_t* file_size);
  int jos_mmap_read(int file_handle, int64_t size, void** q);
  int jos_mmap_write(int file_handle, int64_t size, void** q);
  void jos_munmap(void*);
  */
  int f2 = 0;
  char *p = 0;
  int err = 0;
  int64_t file_size = 0;

  FILE *f1 = fopen("1", "wb");
  fwrite("1", 1, 1, f1);
  fclose(f1);

  err = jos_open_file_read("1", &f2);
  if (err)
    printf("%d %d\n", f2, err);
  assert(!err);
  assert(f2);

  err = jos_get_file_size(f2, &file_size);
  assert(!err);
  assert(file_size == 1);

  jos_close_file(f2);
  err = jos_open_file_read("1", &f2);
  if (err)
    printf("%d %d\n", f2, err);
  assert(!err);
  assert(f2);

  err = jos_open_file_write("1", &f2);
  if (err)
    printf("%d %d\n", f2, err);
  assert(!err);
  assert(f2);

  err = jos_set_file_size(f2, 2);
  assert(!err);

  err = jos_get_file_size(f2, &file_size);
  assert(!err);
  assert(file_size == 2);

  p = 0;
  err = jos_mmap_read(f2, 1, (void **)&p);
  jos_close_file(f2);
  if (err)
    printf("%d %d\n", f2, err);
  assert(p);
  assert(!err);
  assert(*p == '1');
  jos_munmap(p, 1);

  err = jos_open_file_write("1", &f2);
  if (err)
    printf("%d %d\n", f2, err);
  assert(!err);
  assert(f2);

  err = jos_mmap_write(f2, 1, (void **)&p);
  if (err)
    printf("%d\n", err);
  assert(!err);
  assert(p);
  assert(*p == '1');
  *p = '2';
  assert(*p == '2');

  err = jos_open_file_read("1", &f2);
  if (err)
    printf("%d\n", err);
  assert(!err);
  assert(f2);
  err = jos_mmap_read(f2, 1, (void **)&p);
  if (err)
    printf("%d\n", err);
  assert(!err);
  assert(p);
  assert(*p == '2');
  jos_munmap(p, 1);
}
