/*
 * A hash table.
 *
 * Array of buckets, prime count.
 * Collisions are doubly linked list (jlist_t).
 * Full hash code is remembered for quick comparison.
 */
#if !JHASH_H
#define JHASH_H 1

#include "jhashcode.h"
#include "jlist.h"
#include <stddef.h>
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

typedef struct jhash_t jhash_t;
typedef struct jhash_init_t jhash_init_t;

/* TODO: is this jtype? */
struct jhash_init_t {
  void *context;
  jhashcode_t (*hash)(void *context, void *data);
  int (*compare)(void *context, void *a, void *b);
  int (*copy_uninit)(void *context, void *to, void *from);
  int (*copy_over)(void *context, void *to, void *from);
};

struct jhash_t {
  /* public */
  size_t element_count;
  /* private */
  jhash_init_t init;
  size_t bucket_count;
  jlist_t *buckets; /* array[bucket_count] of lists of jlist_t */
};

typedef struct jhash_lookup_t {
  /* inout
  Upon call, is the key (prefix of data) to lookup.
  Upon return, is the equivalent key found, not necessarily the same,
  unlikely the same, since hash table can copy in the data.
  i.e. This is an interior pointer into the table.
  */
  void *data;
  size_t size; /* for insert */

  /* internal
  If key is not found, this helps insert.
  If found, this helps remove.
  */
  jhashcode_t hashcode;
  jlist_t *element;

} jhash_lookup_t;

/* jhash_enum_t e = {h};
   for (;jhash_enum(e);) {
     e->data
   }
*/
typedef struct jhash_enum_t {
  jhash_t *hash;

  /* public */
  void *data;

  /* internal */
  size_t bucket_index;
  jlist_t *list;
  jlist_t *element;
} jhash_enum_t;

int jhash_new(jhash_init_t *init, jhash_t **);
int jhash_lookup(jhash_t *hash, jhash_lookup_t *);
int jhash_lookup_and_insert_new(jhash_t *hash, jhash_lookup_t *lookup);
int jhash_lookup_and_insert_replace(jhash_t *hash, jhash_lookup_t *lookup);
int jhash_insert_new_after_lookup(jhash_t *hash, jhash_lookup_t *lookup);
int jhash_lookup_and_remove(jhash_t *hash, jhash_lookup_t *lookup);
int jhash_remove_after_lookup(jhash_t *hash, jhash_lookup_t *lookup);
int jhash_enum(jhash_enum_t *e);

#if __cplusplus
} /* extern "C" */
#endif
#endif
