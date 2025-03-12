/*
 * A hash table.
 *
 * Array of buckets, prime count.
 * Collisions are doubly linked list (jlist_t).
 * Full hash code is remembered for quick comparison.
 */
#include "jhash.h"
#include "jbase_of.h"
#include "jcharp.h"
#include "jcount.h"
#include "jerr.h"
#include "jlist.h"
#include "jmem.h"
#include <stddef.h>
#include <stdlib.h> /* TODO replace */

#pragma warning(disable : 4100) /* unused parameter */

#define JHASH_PRIMES                                                           \
  JHASH_PRIME(53)                                                              \
  JHASH_PRIME(163)                                                             \
  JHASH_PRIME(491)                                                             \
  JHASH_PRIME(1481)                                                            \
  JHASH_PRIME(4447)                                                            \
  JHASH_PRIME(13367)                                                           \
  JHASH_PRIME(40111)                                                           \
  JHASH_PRIME(120349)                                                          \
  JHASH_PRIME(361069)

static size_t jhash_primes[] = {
#define JHASH_PRIME(x) x,
    JHASH_PRIMES
#undef JHASH_PRIME
};

/* Mod is expensive. Mod by a constant is not. */
static uint64_t jhash_mod(uint64_t a, uint64_t b) {
  switch (b) {
#define JHASH_PRIME(x)                                                         \
  case x:                                                                      \
    return a % x;
    JHASH_PRIMES
#undef JHASH_PRIME
  default:
    return a % b;
  }
}

typedef struct jhash_data_t {
  jlist_t list; /* list of jhash_data_t for hash collision */
  jhashcode_t hashcode;
  size_t size;
} jhash_data_t;

int jhash_new(jhash_init_t *init, jhash_t **hash) {
  *hash = (jhash_t *)calloc(1, sizeof(jhash_t));
  if (!*hash)
    return jerr_out_of_memory;
  (*hash)->init = *init;
  return 0;
}

/* Delete a hashtable partially, having reused the data pairs
during rehash. That is delete the toplevel buckets.
*/
void jhash_delete_shallow(jhash_t *hash) {
  /* TODO data pairs moved to new table in rehash */
}

/* Delete an entire hashtable. */
void jhash_delete(jhash_t *hash) { /* TODO */ }

/* Lookup data in a hashtable. */
int jhash_lookup(jhash_t *hash, jhash_lookup_t *lookup) {
  jhash_init_t *const init = &hash->init;
  jlist_t *bucket = {0};
  jlist_t *element = {0};
  jhash_data_t *data = {0};
  jhashcode_t const hashcode = init->hash(init->context, lookup->data);

  lookup->hashcode = hashcode;
  if (hash->bucket_count) {
    bucket = &hash->buckets[hashcode % hash->bucket_count];
    element = bucket->flink;
    lookup->hashcode = hashcode;
    while (element && element != bucket) {
      data = JBASE_OF(jhash_data_t, list, element);
      if (hashcode == data->hashcode &&
          0 == init->compare(init->context, data + 1, lookup->data)) {
        lookup->data = (1 + data);
        lookup->element = element;
        return 0;
      }
      element = element->flink;
    }
  }
  return jerr_not_found;
}

static size_t jhash_decide_bucket_count(size_t element_count) {
  size_t i;
  for (i = 0; i < JCOUNT(jhash_primes); ++i) {
    if (element_count < jhash_primes[i])
      return jhash_primes[i];
  }
  return jhash_primes[JCOUNT(jhash_primes) - 1];
}

/* TODO: Rehash for high load. */
static int jhash_new_buckets(jhash_t *hash, size_t bucket_count) {
  if (hash->buckets)
    return 0;
  jlist_t *buckets = (jlist_t *)calloc(bucket_count, sizeof(jlist_t));
  if (!buckets)
    return jerr_out_of_memory;
  hash->buckets = buckets;
  hash->bucket_count = bucket_count;
  return 0;
}

int jhash_enum(jhash_enum_t *e) {
  jhash_data_t *data;
  if (e->element) {
    if ((e->element = e->element->flink) != e->list) {
    ret:
      data = JBASE_OF(jhash_data_t, list, e->element);
      e->data = (data + 1);
      return 1;
    } else {
      ++(e->bucket_index);
    }
  }
  for (; e->bucket_index < e->hash->bucket_count; ++(e->bucket_index)) {
    if (!jlist_is_empty(e->list = &e->hash->buckets[e->bucket_index])) {
      e->element = e->list->flink;
      goto ret;
    }
  }
  return 0;
}

/* When element count grows too much relative to bucket count, rehash.
 * This is to reduce linear search for hash collisions.
 */
#if 0 /* WIP */
int jhash_rehash(jhash_t *hash, size_t new_element_count) {
	int err = 0;
	jhash_t new_hash = {0};
	size_t old_bucket_count = hash->bucket_count;
	size_t new_bucket_count = jhash_decide_bucket_count(new_element_count * 2);
	if (old_bucket_count == new_bucket_count)
		return 0;

	new_hash.init = hash->init;
	if (err = jhash_new_buckets(&new_hash, new_bucket_count))
		return err;
fail:
	jhash_delete(new_hash);
}
#endif

/* TODO: useful for reuse during rehash */
static int jhash_insert_data(jhash_t *hash, jhash_lookup_t *lookup,
                             jhash_data_t *data)
/* Internal common code for inserting new data.
 */
{
  jlist_append(&hash->buckets[lookup->hashcode % hash->bucket_count],
               &data->list);
  ++(hash->element_count);
  return 0;
}

int jhash_insert_new_after_lookup(jhash_t *hash, jhash_lookup_t *lookup)
/* Having already looked up an element and presumably it was not found,
   insert new. This reduces excess calls to hash in common patterns.
*/
{
  int err = {0};
  jhash_init_t *const init = &hash->init;
  jhash_data_t *data =
      (jhash_data_t *)calloc(1, sizeof(jhash_data_t) + lookup->size);
  if (!data)
    return jerr_out_of_memory;
  data->hashcode = lookup->hashcode;
  init->copy_uninit(init->context, (data + 1), lookup->data);
  data->size = lookup->size;

  if ((err = jhash_new_buckets(
           hash, jhash_decide_bucket_count(2 * (hash->element_count + 1))))) {
    free(data);
    return err;
  }

  return jhash_insert_data(hash, lookup, data);
}

int jhash_lookup_and_insert_new(jhash_t *hash, jhash_lookup_t *lookup)
/* Lookup an element and if it is there, return jerr_found.
   It not found, insert it, and return jerr_not_found.
   Wierd to always error?
*/
{
  jhash_lookup_t existing = *lookup;
  if (!jhash_lookup(hash, &existing))
    return jerr_found;
  lookup->hashcode = existing.hashcode;
  int err = jhash_insert_new_after_lookup(hash, lookup);
  return err ? err : jerr_not_found;
}

int jhash_lookup_and_insert_replace(jhash_t *hash, jhash_lookup_t *lookup)
/* Lookup an element and if it is there, replace it, if not add new.
   Return value jerr_found if it was found and replaced.
   Return value jerr_not_found if not found and new added.
   Wierd to always error?
*/
{
  jhash_lookup_t existing = *lookup;
  jhash_init_t *const init = &hash->init;
  jhash_lookup(hash, &existing);
  if (existing.data && existing.size == lookup->size) {
    init->copy_over(init->context, existing.data, lookup->data);
    return jerr_found;
  }
  jhash_remove_after_lookup(hash, &existing);
  lookup->hashcode = existing.hashcode;
  int err = jhash_insert_new_after_lookup(hash, lookup);
  return err ? err : jerr_not_found;
}

int jhash_remove_after_lookup(jhash_t *hash, jhash_lookup_t *lookup)
/* Remove data from a hashtable, having already looked it up. */
{
  jlist_t *element = lookup->element;
  if (element) {
    jlist_remove_element(element);
    free(element);
    --(hash->element_count);
  }
  return 0;
}

int jhash_lookup_and_remove(jhash_t *hash, jhash_lookup_t *lookup)
/* Lookup and remove data from a hashtable. */
{
  int err = jhash_lookup(hash, lookup);
  return (err ? err : jhash_remove_after_lookup(hash, lookup));
}
