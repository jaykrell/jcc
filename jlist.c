#include "jlist.h"
/* Intrusive circular doubly linked lists.
   Based on NT and EFI LIST_ENTRY.
*/
void jlist_init(jlist_t *list) {
  list->flink = list;
  list->blink = list;
}

void jlist_init_on_demand(jlist_t *list) {
  if (!list->flink && !list->flink) {
    jlist_init(list);
  }
}

static void jlist_graft(jlist_t *list, jlist_t *element, int append) {
  jlist_t *insert = 0;
  jlist_init_on_demand(list);
  insert = (append ? list->blink : list->flink);
  *(append ? &insert->flink : &insert->blink) = element;
  *(append ? &list->blink : &list->flink) = element;
  *(append ? &element->flink : &element->blink) = list;
  *(append ? &element->blink : &element->flink) = insert;
}

/* Element becomes the new end of the list. */
void jlist_append(jlist_t *list, jlist_t *element) {
  jlist_graft(list, element, 1);
}

/* Element becomes the new start of the list. */
void jlist_prepend(jlist_t *list, jlist_t *element) {
  jlist_graft(list, element, 0);
}

/* Count the elements in the list. This is slow. Use with caution. */
int64_t jlist_size(jlist_t *list) {
  int64_t size = 0;
  if (list) {
    jlist_t *element = list->flink;
    while (element && element != list) {
      ++size;
      element = element->flink;
    }
  }
  return size;
}

jbool jlist_is_empty(jlist_t *list) {
  return !list->flink || list->flink == list;
}

static jlist_t *jlist_remove_element_internal(jlist_t *element) {
  jlist_t *flink = element->flink;
  jlist_t *blink = element->blink;
  flink->blink = blink;
  blink->flink = flink;
  return element;
}

static jlist_t *jlist_remove_element_from_list(jlist_t *list,
                                               jlist_t *element) {
  if (list != element && element) {
    jlist_remove_element_internal(element);
  }
  return element;
}

void jlist_remove_element(jlist_t *element) {
  jlist_remove_element_internal(element);
}

jlist_t *jlist_remove_first(jlist_t *list) {
  return jlist_remove_element_from_list(list, list ? list->flink : 0);
}

jlist_t *jlist_remove_last(jlist_t *list) {
  return jlist_remove_element_from_list(list, list ? list->blink : 0);
}

uint64_t jlist_iterate(jlist_t *list,
                       uint64_t (*callback)(void *context, void *element),
                       void *context, int64_t offset) {
  uint64_t overall_result = 0;
  uint64_t result = 0;
  jlist_t *element = list->flink;
  while (element && element != list) {
    result = callback(context, ((char *)element) - offset);
    overall_result += result;
    element = element->flink;
    if (!result)
      break;
  }
  return result;
}
