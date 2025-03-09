#include "jhash.h"
#include "jlist.h"
#include <stdlib.h> /* TODO replace */
#include "jbase_of.h"
#include "jmem.h"

typedef struct jhash_keyvalue_t
{
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

jhash_t* jhash_new(jhash_init_t* init)
{
    jhash_t* hash = (jhash_t*)calloc(1, sizeof(*hash));
    if (!hash) return hash;
    hash->init = *init;
	return hash;
}

void jhash_lookup(jhash_t* hash, jhash_lookup_t* lookup)
{
	jhash_init_t* const init = &hash->init;
    jlist_t* bucket = {0};
    jlist_t* element = {0};
    jhash_keyvalue_t* keyvalue = {0};
    julong index = {0};
	jvoidp key = lookup->key;
    jhashcode_t const hashcode = init->hash(init->context, key);

    JMEM_ZERO(lookup, sizeof(*lookup));
    lookup->key = 0;
    lookup->value = 0;
    lookup->bucket = 0;
    lookup->index = -1;
    lookup->hashcode = hashcode;

    if (hash->bucket_count == 0)
    {
        return;
    }
    index = (hashcode % hash->bucket_count);
    bucket = &hash->buckets[index];
    element = bucket->flink;
    lookup->index = index;
	lookup->bucket = bucket;
    lookup->hashcode = hashcode;
    while (element != bucket)
    {
		keyvalue = JBASE_OF(jhash_keyvalue_t, list, element);
		if (hashcode == keyvalue->hashcode && 0 == init->compare(init->context, keyvalue + 1, key))
		{
			lookup->key = (1 + keyvalue);
			lookup->value = keyvalue->key_size + (charp)lookup->key;
			lookup->element = element;
			return;
		}
		element = element->flink;
    }
}

void jhash_insert(jhash_t* hash, jhash_lookup_t* lookup)
{
}
