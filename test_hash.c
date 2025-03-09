/* test_hash.c */

#pragma warning(disable:4100) /* unused parameter */
#include "jvoidp.h"
#include "jhash.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

jhashcode_t jhash1(jvoidp context, jvoidp data)
{
    return *(int*)data;
}
static int compare1(jvoidp context, jvoidp data1, jvoidp data2)
{
    int a = *(int*)data1;
    int b =  *(int*)data2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}
static int copy1(jvoidp context, jvoidp data1, jvoidp data2)
{
    *(int*)data1 = *(int*)data2;
    return 0;
}
jhash_init_t init1 = {0, jhash1, compare1, copy1, copy1};

/*************************************************************************************************/

int main(void)
{
    jhash_t* hash1 = {0};
    int key = 1;
    int value = 3;

    {
        jhash_new(&init1, &hash1);
        jhash_lookup_t lookup={&key,sizeof(key),&value,sizeof(value)};
        jhash_lookup(hash1, &lookup);
        assert(hash1->element_count == 0);
        jhash_insert(hash1, &lookup);
        assert(hash1->element_count == 1);
        lookup.key=&key;
        jhash_lookup(hash1, &lookup);
    }
    {
        jhash_lookup_t lookup={&key};
        jhash_lookup(hash1, &lookup);
        key = 2;
        assert(3 == *(int*)lookup.value);
        printf("%d\n", *(int*)lookup.value);
    }
    {
        jhash_lookup_t lookup={&key};
        key = 1;
        jhash_lookup_and_remove(hash1, &lookup);
        assert(hash1->element_count == 0);
    }
}
