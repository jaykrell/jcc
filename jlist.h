#if !JLIST_H
#define JLIST_H 1

/* Intrusive circular doubly linked lists.
   This is the same as NT and EFI LIST_ENTRY, etc. but the only part
   copied/pasted (from memory) is the data structure.
*/

#include "jbool.h"
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

typedef struct jlist_t jlist_t;

struct jlist_t {
  jlist_t *flink; /* forward link */
  jlist_t *blink; /* backward link */
};

void jlist_init(jlist_t *list);

/* Element becomes the new end of the list. */
/* List can be an element or list head. */
void jlist_append(jlist_t *list, jlist_t *element);

/* Element becomes the new start of the list. */
/* List can be an element or list head. */
void jlist_prepend(jlist_t *list, jlist_t *element);

/* Remove and return the first element. */
jlist_t *jlist_remove_first(jlist_t *list);

/* Remove this element from its list. */
void jlist_remove_element(jlist_t *element);

/* Remove and return the last element. */
jlist_t *jlist_remove_last(jlist_t *list);

/* Count the elements in the list. This is slow. Use with caution. */
int64_t jlist_size(jlist_t *list);

/* Does the list have any elements? */
jbool jlist_is_empty(jlist_t *list);

/* Call the callback on all the elements, until it returns 0,
adding up the results. The return value may or may not be useful.
0 terminates enumeration.
Callback can want the list entry or the base, passing the correct
offset, or 0.
*/
uint64_t jlist_iterate(jlist_t *list,
                       uint64_t (*callback)(void *context, void *element),
                       void *context, int64_t offset);
#if __cplusplus
} /* extern "C" */
#endif
#endif
