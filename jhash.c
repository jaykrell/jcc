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
#include <stdlib.h> /* TODO replace */
#include <stddef.h>

#pragma warning(disable:4100) /* unused parameter */

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

typedef struct jhash_keyvalue_t {
  jlist_t list; /* list of jhash_keyvalue_t for hash collision */
  jhashcode_t hashcode;
  size_t key_size;
  size_t value_size;
} jhash_keyvalue_t;

int jhash_new(jhash_init_t *init, jhash_t **hash) {
  *hash = (jhash_t *)calloc(1, sizeof(jhash_t));
  if (!*hash)
    return jerr_out_of_memory;
  (*hash)->init = *init;
  return 0;
}

/* Delete a hashtable partially, having reused the keyvalue pairs
during rehash. That is delete the toplevel buckets.
*/
void jhash_delete_shallow(jhash_t *hash) {
  /* TODO keyvalue pairs moved to new table in rehash */
}

/* Delete an entire hashtable. */
void jhash_delete(jhash_t *hash) { /* TODO */ }

/* Lookup a key in a hashtable. */
int jhash_lookup(jhash_t *hash, jhash_lookup_t *lookup) {
  jhash_init_t *const init = &hash->init;
  jlist_t *bucket = {0};
  jlist_t *element = {0};
  jhash_keyvalue_t *keyvalue = {0};
  jvoidp key = lookup->key;
  jhashcode_t const hashcode = init->hash(init->context, key);

  lookup->hashcode = hashcode;
  if (hash->bucket_count) {
    bucket = &hash->buckets[hashcode % hash->bucket_count];
    element = bucket->flink;
    lookup->hashcode = hashcode;
    while (element && element != bucket) {
      keyvalue = JBASE_OF(jhash_keyvalue_t, list, element);
      if (hashcode == keyvalue->hashcode &&
          0 == init->compare(init->context, keyvalue + 1, key)) {
        lookup->key = (1 + keyvalue);
        lookup->value = keyvalue->key_size + (jcharp)lookup->key;
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
  jhash_keyvalue_t *keyvalue;
  if (e->element && (e->element = e->element->flink) != e->list) {
  ret:
    keyvalue = JBASE_OF(jhash_keyvalue_t, list, e->element);
    e->key = (keyvalue + 1);
    e->value = keyvalue->key_size + (jcharp)e->key;
    return 1;
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
static int jhash_insert_keyvalue(jhash_t *hash, jhash_lookup_t *lookup,
                                 jhash_keyvalue_t *keyvalue) {
  jlist_append(&hash->buckets[lookup->hashcode % hash->bucket_count],
               &keyvalue->list);
  ++(hash->element_count);
  return 0;
}

/* Insert a key+value into hashtable, having already called lookup to hash it.
 */
int jhash_insert(jhash_t *hash, jhash_lookup_t *lookup) {
  int err = {0};
  jhash_init_t *const init = &hash->init;
  jhash_keyvalue_t *keyvalue = (jhash_keyvalue_t *)calloc(
      1, sizeof(jhash_keyvalue_t) + lookup->key_size + lookup->value_size);
  if (!keyvalue)
    return jerr_out_of_memory;
  keyvalue->hashcode = lookup->hashcode;
  init->copy_key(init->context, (keyvalue + 1), lookup->key);
  init->copy_value(init->context, lookup->key_size + (jcharp)(keyvalue + 1),
                   lookup->value);
  keyvalue->key_size = lookup->key_size;
  keyvalue->value_size = lookup->value_size;

  if (err = jhash_new_buckets(
          hash, jhash_decide_bucket_count(2 * (hash->element_count + 1)))) {
    free(keyvalue);
    return err;
  }

  return jhash_insert_keyvalue(hash, lookup, keyvalue);
}

/* Remove a key from a hashtable, having already looked it up. */
int jhash_remove_after_lookup(jhash_t *hash, jhash_lookup_t *lookup) {
  jlist_t *element = lookup->element;
  if (element) {
    jlist_remove_element(element);
    free(element);
    --(hash->element_count);
  }
  return 0;
}

/* Lookup and remove a key from a hashtable. */
int jhash_lookup_and_remove(jhash_t *hash, jhash_lookup_t *lookup) {
  int err = jhash_lookup(hash, lookup);
  return (err ? err : jhash_remove_after_lookup(hash, lookup));
}
