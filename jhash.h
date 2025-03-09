#ifndef JHASH_H
#define JHASH_H 1

#include "julong.h"
#include "jvoidp.h"
#include "jlist.h"

typedef struct jhash_t          jhash_t;
typedef struct jhash_init_t     jhash_init_t;
typedef struct jhash_keyvalue_t jhash_keyvalue_t;

typedef julong jhashcode_t;

/* todo: is this jtype? */
struct jhash_init_t
{
	jvoidp context;
    jhashcode_t (*hash)(jvoidp context, jvoidp data);
    int (*compare)(jvoidp context, jvoidp a, jvoidp b);
    int (*copy)(jvoidp context, jvoidp to, jvoidp from);
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

    /* out
	Upon return, address of value found, or null
	Since the table stores value and this points to the value, null is represented, by a pointer to it.
	i.e. null unambiguously means not found
	*/
    jvoidp value;

    /* internal
	If key is not found, this helps insert.
	If found, this helps remove.
	*/
    jhashcode_t hashcode;
    jlist_t* bucket;
    jlist_t* element;
    jlong index;

} jhash_lookup_t;

jhash_t* jhash_new(jhash_init_t* init);
void jhash_lookup(jhash_t* hash, jhash_lookup_t*);
void jhash_insert(jhash_t* hash, jhash_lookup_t*);

#endif
