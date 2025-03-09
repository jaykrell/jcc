#include "jhash.h"
#include "jlist.h"
#include <stdlib.h> /* TODO replace */
#include "jbase_of.h"
#include "jmem.h"
#include "jerr.h"
#include "jcharp.h"

typedef struct jhash_keyvalue_t {
    jlist_t list; /* list of jhash_keyvalue_t */
    jhashcode_t hashcode;
    jlong key_size;
    jlong value_size;
} jhash_keyvalue_t;

struct jhash_t
{
    jhash_init_t init;
    julong bucket_count;
    jlist_t* buckets; /* array[bucket_count] of lists of jhash_keyvalue_t */
    julong element_count;
};

int jhash_new(jhash_init_t* init, jhash_t** hash)
{
    *hash = (jhash_t*)calloc(1, sizeof(jhash_t));
    if (!*hash) return jerr_out_of_memory;
    (*hash)->init = *init;
    return 0;
}

int jhash_lookup(jhash_t* hash, jhash_lookup_t* lookup)
{
    jhash_init_t* const init = &hash->init;
    jlist_t* bucket = {0};
    jlist_t* element = {0};
    jhash_keyvalue_t* keyvalue = {0};
    jvoidp key = lookup->key;
    jhashcode_t const hashcode = init->hash(init->context, key);

    lookup->hashcode = hashcode;
    if (hash->bucket_count)
    {
        bucket = &hash->buckets[hashcode % hash->bucket_count];
        element = bucket->flink;
        lookup->hashcode = hashcode;
        while (element && element != bucket)
        {
            keyvalue = JBASE_OF(jhash_keyvalue_t, list, element);
            if (hashcode == keyvalue->hashcode && 0 == init->compare(init->context, keyvalue + 1, key))
            {
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

static int jhash_new_buckets(jhash_t* hash, int count)
{
    if (hash->buckets)
        return 0;
    jlist_t* buckets = (jlist_t*)calloc(count, sizeof(jlist_t));
    if (!buckets)
        return jerr_out_of_memory;
    hash->buckets = buckets;
    hash->bucket_count = count;
    return 0;
}

int jhash_insert(jhash_t* hash, jhash_lookup_t* lookup)
{
    int err = {0};
    jhash_init_t* const init = &hash->init;
    jhash_keyvalue_t* keyvalue = (jhash_keyvalue_t*)calloc(1, sizeof(jhash_keyvalue_t) + lookup->key_size + lookup->value_size);
    if (!keyvalue)
        return jerr_out_of_memory;
    keyvalue->hashcode = lookup->hashcode;
    init->copy_key(init->context, (keyvalue + 1), lookup->key);
    init->copy_value(init->context, lookup->key_size + (jcharp)(keyvalue + 1), lookup->value);
    keyvalue->key_size = lookup->key_size;
    keyvalue->value_size = lookup->value_size;

    if ((err = jhash_new_buckets(hash, 999)))
        return err;

    jlist_append(&hash->buckets[lookup->hashcode % hash->bucket_count], &keyvalue->list);
    return 0;
}
