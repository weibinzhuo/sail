
#include <sail/base/dict.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void testAdd()
{
	const char *key = "hello";
	const char *val = "world";

	Dict d;
	d.dictAdd(key, val);
	DictEntry *entry = d.dictFind(key);
    DICT_NOTUSED(entry);

	assert(memcmp((char *)entry->val, val, strlen(val)) == 0);
	//printf("val: %s\n", (char *)entry->val);
}

void testReplace()
{
    const char *key = "switch";
    const int v1 = 1;
    const int v2 = 2;

    Dict d;
    d.dictAdd(key, (const void *)&v1);
    d.dictReplace(key, (void *)&v2);

    DictEntry *entry = d.dictFind(key);
    int val = *(int *)entry->val;
    assert(v2 == val);
}

int main(int argc, char **argv)
{
    DICT_NOTUSED(argc);
    DICT_NOTUSED(argv);

	testAdd();
    testReplace();
	return 0;
}
