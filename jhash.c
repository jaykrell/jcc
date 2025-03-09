#include "jhash.h"
#include "jlist.h"

struct jhash_keyvalue_t
{
    jlist_t list; /* list of jhash_keyvalue_t */
	jhashcode_t hashcode;
    jlong key_size;
    jlong value_size;
};

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

void jhash_lookup(jhash_t* hash, jhash_lookup_data_t* lookup_data);
{
	jhash_init_t* const init = &hash->init;
    jlist_t* bucket = {0};
    jlist_t* element = {0};
    jhash_keyvalue_t* keyvalue = {0};
    jhashcode_t const hashcode = init->hash(init->context, lookup_data->key);
    julong const index = (hashcode % hash->bucket_count);

    JMEM_ZERO(lookup_data, sizeof(*lookup_data));
    lookup_data->hashcode = hashcode;
    lookup_data->bucket = 0;

    if (hash->bucket_count == 0)
    {
        return;
    }
    bucket = &hash->buckets[index];
    element = bucket->flink;
    while (element != bucket)
    {
		keyvalue = JBASE_OF(jhash_keyvalue_t, list, bucket);
		if (keyvalue->hashcode && 0 == init->compare(init->context, keyvalue + 1, lookup_data->key))
		{
			lookup_data->value = keyvalue->key_size + (charp)(1 + keyvalue);
			return;
		}
		element = element->flink;
    }
}

void jhash_insert(jhash_t* hash, jhash_lookup_data_t*);
{
}
