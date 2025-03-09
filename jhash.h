#ifndef JHASH_H
#define JHASH_H 1

#include "julong.h"
#include "jvoidp.h"
#include "jlist.h"
#include "jhashcode.h"

typedef struct jhash_t          jhash_t;
typedef struct jhash_init_t     jhash_init_t;
typedef struct jhash_keyvalue_t jhash_keyvalue_t;

/* todo: is this jtype? */
struct jhash_init_t {
    jvoidp context;
    jhashcode_t (*hash)(jvoidp context, jvoidp data);
    int (*compare)(jvoidp context, jvoidp a, jvoidp b);
    int (*copy_key)(jvoidp context, jvoidp to, jvoidp from);
    int (*copy_value)(jvoidp context, jvoidp to, jvoidp from);
};

struct jhash_t {
	/* public */
    julong element_count;
	/* private */
    jhash_init_t init;
    julong bucket_count;
    jlist_t* buckets; /* array[bucket_count] of lists of jhash_keyvalue_t */
};

typedef struct jhash_lookup_t
{
    /* inout
    Upon call, is the key to lookup.
    Upon return, is the equivalent key found, not necessarily the same,
    unlikely the same, since hash table can copy in the data.
    i.e. This is an interior pointer into the table.
    */
    jvoidp key;
    jlong key_size; /* for insert */

    /* lookup out
    Upon return, address of value found, or null
    Since the table stores value and this points to the value, null is represented, by a pointer to it.
    i.e. null unambiguously means not found

    insert in
    */
    jvoidp value;
    jlong value_size; /* for insert */

    /* internal
    If key is not found, this helps insert.
    If found, this helps remove.
    */
    jhashcode_t hashcode;
    jlist_t* element;

} jhash_lookup_t;

int jhash_new(jhash_init_t* init, jhash_t**);
int jhash_lookup(jhash_t* hash, jhash_lookup_t*);
int jhash_insert(jhash_t* hash, jhash_lookup_t*);
int jhash_lookup_and_remove(jhash_t* hash, jhash_lookup_t* lookup);
int jhash_remove_after_lookup(jhash_t* hash, jhash_lookup_t* lookup);

#endif
