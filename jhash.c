#include "jhash.h"

struct jhash
{
    jhash_init init;
    julong bucket_count;
    jhash_bucket* buckets;
    julong element_count;
};

typedef struct jhash_keyvalue jhash_keyvalue;

struct jhash_keyvalue
{
    jhash_keyvalue* next;
    jssize key_size;
    jssize value_size;
};

struct jhash_bucket
{
};

jhash* jhash_new(jhash_init* init)
{
    jhash* hash = (jhash*)calloc(1, sizeof(*hash));
    if (!hash) return hash;
    hash->init = *init;
}

void jhash_lookup(jhash* hash, jhash_lookup_data* lookup_data);
{
    jhashcode hashcode = hash->init.hsah(lookup_data->key);
    julong index = (hashcode % hash->bucket_count);

    jzeromem(lookup_data, sizeof(lookup_data));
    lookup_data->hashcode = hashcode;
    lookup_data->bucket = 0;;

    if (hash->bucket_count == 0)
    {
        return;
    }
    jbucket* bucket = &hash->buckes
    while (1)
    {

    }
}

void jhash_insert(jhash* hash, jhash_lookup_data*);
{
}
