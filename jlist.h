#ifndef JLIST
#define JLIST

/* Based on NT and EFI LIST_ENTRY, etc. */

#include "jbase.h"

typedef struct jlist_t jlist_t;

struct jlist_t {
	jlist_t* next;
	jlist_t* prev;
};

void jlist_init(jlist_t* list);

/* Element becomes the new start of the list. */
void jlist_append(jlist_t* list, jlist_t* element);

void jlist_prepend(jlist_t* list, jlist_t* element);

/* Count the elements in the list. This is slow. Use with caution. */
jlong jlist_size(jlist_t* list);

#endif
