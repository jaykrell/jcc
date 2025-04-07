/* test_hash.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jhash.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

typedef struct int_int {
  int key, value;
} int_int;

static jhashcode_t hash(void *context, void *data) {
  return ((int_int *)data)->value;
}

static int compare(void *context, void *data1, void *data2) {
  int a = ((int_int *)data1)->value;
  int b = ((int_int *)data2)->value;
  if (a < b)
    return -1;
  if (a > b)
    return 1;
  return 0;
}

static int copy(void *context, void *data1, void *data2) {
  *(int_int *)data1 = *(int_int *)data2;
  return 0;
}

jhash_init_t init1 = {0, hash, compare, copy};

/*************************************************************************************************/

int main(void) {
  jhash_t *hash1 = {0};
  int_int data = {1, 3};

  jhash_new(&init1, &hash1);

  {
    jhash_enum_t e = {hash1};
    printf("enumerate empty hash\n");
    for (; jhash_enum(&e);)
      printf("%d = %d\n", ((int_int *)e.data)->key, ((int_int *)e.data)->value);

    /* add one element to hashtable */
    jhash_lookup_t lookup = {&data, sizeof(data)};
    jhash_lookup(hash1, &lookup);
    assert(hash1->element_count == 0);
    jhash_insert_new_after_lookup(hash1, &lookup);
    assert(hash1->element_count == 1);
    lookup.data = &data;
    jhash_lookup(hash1, &lookup);
  }

  printf("enumerate one element hash\n");
  {
    jhash_enum_t e = {hash1};
    for (; jhash_enum(&e);)
      printf("%d = %d\n", ((int_int *)e.data)->key, ((int_int *)e.data)->value);
  }

  /* lookup one element */
  {
    jhash_lookup_t lookup = {&data};
    jhash_lookup(hash1, &lookup);
    data.key = 2;
    assert(3 == ((int_int *)lookup.data)->value);
    printf("%d\n", ((int_int *)lookup.data)->value);
  }

  /* remove one element */

  {
    jhash_lookup_t lookup = {&data};
    data.key = 1;
    jhash_lookup_and_remove(hash1, &lookup);
    assert(hash1->element_count == 0);
  }

  printf("enumerate few element hash\n");
  {
    jhash_lookup_t lookup = {&data, sizeof(data)};
    data.key = 10;
    data.value = 100;
    jhash_lookup_and_insert_new(hash1, &lookup);
    data.key = 20;
    data.value = 200;
    jhash_lookup_and_insert_new(hash1, &lookup);
    data.key = 30;
    data.value = 300;
    jhash_lookup_and_insert_new(hash1, &lookup);
    data.key = 40;
    data.value = 400;
    jhash_lookup_and_insert_new(hash1, &lookup);

    jhash_enum_t e = {hash1};
    for (; jhash_enum(&e);)
      printf("%d = %d\n", ((int_int *)e.data)->key, ((int_int *)e.data)->value);
  }
}
