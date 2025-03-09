#ifndef JHASH_H
#define JHASH_H 1

#include "julong.h"

typedef struct jhash_t          jhash_t;
typedef struct jhash_init_t     jhash_init_t;
typedef struct jhash_keyvalue_t jhash_keyvalue_t;

typedef julong jhashcode_t;

/* todo: is this jtype? */
struct jhash_init_t
{
	voidp context;
    jhashcode_t (*hash)(voidp context, charp data);
    int (*compare)(voidp context, charp a, charp b);
    int (*copy)(voidp context, charp to, charp from);
};

typedef struct jhash_lookup_data_t
{
    /* inout */
    charp* key;

    /* out */
    charp* value;

    /* internal */
    jhashcode_t hashcode;
    jhash_keyvalue_t* bucket;

} jhash_lookup_data_t;

jhash_t* jhash_new(jhash_init_t* init);
void jhash_lookup(jhash_t* hash, jhash_lookup_data_t*);
void jhash_insert(jhash_t* hash, jhash_lookup_data_t*);

#endif
