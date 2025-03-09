#include "jssize.h"
#include "jlist.h"

void jlist_init(jlist_t* list)
{
	list->flink = list;
	list->blink = list;
}

static void jlist_graft(jlist_t* list, jlist_t* element, int append)
{
	jlist_t* insert = (append ? list->blink : list->flink);
	*(append ? &insert->flink : &insert->blink) = element;
	*(append ? &list->blink : &list->flink) = element;
	*(append ? &element->flink : &element->blink) = list;
	*(append ? &element->blink : &element->flink) = insert;
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
	jlist_t* element = list->flink;
	while (element != list)
	{
		++size;
		element = element->flink;
	}
	return size;
}

jbool jlist_is_empty(jlist_t* list)
{
	return list->flink && list->flink != list->blink;
}

jlist_t* jlist_remove_element(jlist_t* element)
{
	jlist_t* flink = element->flink;
	jlist_t* blink = element->blink;
	flink->blink = blink;
	blink->flink = flink;
	return element;
}

jlist_t* jlist_remove_first(jlist_t* list)
{
	return jlist_remove_element(list->flink);
}

jlist_t* jlist_remove_last(jlist_t* list)
{
	return jlist_remove_element(list->blink);
}

jlong jlist_iterate(jlist_t* list,
					jlong (*callback)(voidp context, voidp element),
					voidp context,
					jlong offset)
{
	jlong overall_result = 0;
	jlong result = 1;
	jlist_t* element = list->flink;
	while (result && element != list)
	{
		result = callback(context, ((char*)element) - offset);
		overall_result += result;
	}
	return result;
}
