#ifndef jhash_h
#define jhash_h 1

#include "jint.h"

typedef struct jhash        jhash;
typedef struct jhash_init   jhash_init;
typedef struct jhash_bucket jhash_bucket;

typedef julong jhashcode;

/* todo: is this jtype? */
struct jhash_init
{
    jhashcode (*hash)(charp data);
    int (*compare)(charp a, charp b);
    int (*copy)(charp to, charp from);
};

typedef struct jhash_lookup_data
{
    /* inout */
    charp* key;

    /* out */
    charp* value;

    /* internal */
    jhashcode hashcode;
    jhash_bucket* bucket;

} jhash_lookup_data;

jhash* jhash_new(jhash_init* init);
void jhash_lookup(jhash* hash, jhash_lookup_data*);
void jhash_insert(jhash* hash, jhash_lookup_data*);

#endif
