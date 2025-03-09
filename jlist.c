#include "jlist.h"

void jlist_init(jlist_t* list)
{
	list->next = list;
	list->prev = list;
}

static void jlist_graft(jlist_t* list, jlist_t* element, int append)
{
	list_t_t insert = (append ? list->prev : list->next);
	*(append ? &insert->next : &insert->prev) = element;
	*(append ? &list->prev : &list->next) = element;
	*(append ? &element->next : &element->prev) = list;
	*(append ? &element->prev : &element->next) = insert;
}

/* Element becomes the new end of the list. */
void jlist_append(jlist_t* list, jlist_t* element)
{
	jlist_graft (list, element, 1);
}

/* Element becomes the new start of the list. */
void jlist_prepend(jlist_t* list, jlist_t* element)
{
	jlist_graft (list, element, 0);
}

/* Count the elements in the list. This is slow. Use with caution. */
jlong jlist_size(jlist_t* list)
{
	jlong size = 0;
	list_t* element = list->next;
	while (element != list)
	{
		++size;
		element = element->next;
	}
	return size;
}
