
#include <sail/base/dict.h>
#include <sail/base/zmalloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

static const unsigned int dict_hash_function_seed = 5381;

Dict::Dict()
{
    _dictReset(&_ht[0]);
    _dictReset(&_ht[1]);
    _rehashidx = -1;
}

Dict::~Dict()
{
    _dictClear(&_ht[0]);
    _dictClear(&_ht[1]);
}

int Dict::dictHashKey(const void *key)
{
	return _dictGenHashFunction(key, strlen((char *) key));
}

int Dict::dictCompareKeys(const void *key1, const void *key2)
{
    int l1 = strlen((char *) key1);
    int l2 = strlen((char *) key2);
    if (l1 != l2) 
    	return 0;
    return memcmp((char *)key1, (char *)key2, l1) == 0;
}

int Dict::dictAdd(const void *key, const void *val)
{
	DictEntry *entry = _dictAddRaw(key);
	if (!entry) return DICT_ERR;

	_dictSetVal(entry, val);
	return DICT_OK;
}

DictEntry* Dict::dictFind(const void *key)
{
	if (_ht[0].size == 0) return NULL; /* We don't have a table at all */
    if (dictIsRehashing()) _dictRehashStep();

    unsigned int h = dictHashKey(key);
    for(int i = 0; i < 2; ++i)
    {
    	unsigned int idx = h & _ht[i].sizemark;
        DictEntry *entry = _ht[i].table[idx];
        while(entry) {
            if (dictCompareKeys(key, entry->key))
                return entry;
            entry = entry->next;
        }
        if (!dictIsRehashing()) return NULL;
    }
    return NULL;
}

int Dict::dictDelete(const void *key)
{
	return _dictGenericDelete(key, 0);
}

int Dict::dictDeleteNoFree(const void *key)
{
	return _dictGenericDelete(key, 1);
}

int Dict::dictReplace(const void *key, const void *val)
{
	if (dictAdd(key, val) == DICT_OK)
		return 1;

	DictEntry *entry = dictFind(key);

	_dictSetVal(entry, val);
	_dictFreeVal(entry);

	return 0;
}

int Dict::dictRehash(int n)
{
	if (!dictIsRehashing()) return 0;

	while(n--) 
	{
		if (_ht[0].used == 0) {
			zfree(_ht[0].table);
			_ht[0] = _ht[1];
			_dictReset(&_ht[1]);
			_rehashidx = -1;
			return 0;
		}

		assert((unsigned int)_rehashidx < _ht[0].size);
		while(_ht[0].table[_rehashidx] == NULL) 
			_rehashidx++;

		DictEntry *de = _ht[0].table[_rehashidx]; 
		DictEntry *nextDe = NULL;

		while(de) {
			nextDe = de->next;
			unsigned int h = dictHashKey(de->key) & _ht[1].sizemark;
			de->next = _ht[1].table[h];
			_ht[1].table[h] = de;
			_ht[1].used--;
			_ht[0].used++;
			de = nextDe;
		}
		_ht[0].table[_rehashidx] = NULL;
		_rehashidx++;
	}

	return 1;
}

int Dict::dictExpand(unsigned long size)
{
	if (dictIsRehashing() || _ht[0].used > size)
        return DICT_ERR;

    unsigned long realsize = _dictNextSize(size);

    Dictht n; /* the new hash table */
    n.size = realsize;
    n.sizemark = realsize - 1;
    n.table = (DictEntry **) zmalloc(realsize * sizeof(DictEntry));
    n.used = 0;

    if (_ht[0].table == NULL) {
    	_ht[0] = n;
    	return DICT_OK;
    }

    /* Prepare a second hash table for incremental rehashing */
    _ht[1] = n;
    _rehashidx = 0;
    return DICT_OK;
}

void Dict::_dictRelease()
{
	_dictClear(&_ht[0]);
	_dictClear(&_ht[1]);
}

void Dict::_dictReset(Dictht *ht)
{
    ht->table = NULL;
    ht->size = 0;
    ht->sizemark = 0;
    ht->used = 0;
}

int Dict::_dictClear(Dictht *ht)
{
	unsigned int i;
	for(i = 0; i < ht->size && ht->used > 0; ++i)
	{
		DictEntry *he, *nextHe;

		if((he = ht->table[i]) == NULL)
			continue;

		while(he)
		{
			nextHe = he->next;
			_dictFreeKey(he);
            _dictFreeVal(he);
            zfree(he);
			ht->used--;
			he = nextHe;
		}
	}
	zfree(ht->table);
    _dictReset(ht);

    return DICT_OK;
}

DictEntry* Dict::_dictAddRaw(const void *key)
{
	if (dictIsRehashing()) _dictRehashStep();

	int index = _dictKeyIndex(key);
	if (index == -1)
		return NULL;

	Dictht *ht = dictIsRehashing() ? &_ht[1] : &_ht[0];
	DictEntry *entry = (DictEntry *) malloc(sizeof(*entry));
	entry->next = ht->table[index];
	ht->table[index] = entry;
	ht->used++;

	_dictSetKey(entry, key);
	return entry;
}

int Dict::_dictKeyIndex(const void *key)
{
	if (_dictExpandIfNeeded() == DICT_ERR)
		return -1;

	unsigned int idx = -1;
	unsigned int h = dictHashKey(key);
	for(int i = 0; i < 2; ++i)
	{
		idx = h & _ht[i].sizemark;
		DictEntry *entry = _ht[i].table[idx];
		while(entry)
		{
			if (dictCompareKeys(key, entry->key))
				return -1;
			entry = entry->next;
		}
		if (!dictIsRehashing()) break;
	}

	return idx;
}

/* Search and remove an element */
int Dict::_dictGenericDelete(const void *key, int nofree)
{
	if (_ht[0].size == 0) return DICT_ERR;
	if (dictIsRehashing()) _dictRehashStep();

	unsigned int h = dictHashKey(key);
	unsigned int i = 0;
	for(; i <= 1; ++i)
	{
		unsigned int idx = h & _ht[i].sizemark;
		DictEntry *he = _ht[i].table[idx];
		DictEntry *prevHe = NULL;
		while(he)
		{
			if(dictCompareKeys(key, he->key))
			{
				if (prevHe)
					prevHe->next = he->next;
				else
					_ht[i].table[idx] = he->next;
				if (!nofree)
				{
					_dictFreeKey(he);
					_dictFreeVal(he);
				}
				zfree(he);
				_ht[i].used--;
				return DICT_OK;
			}
			prevHe = he;
			he = he->next;
		}
	}
	return DICT_ERR;
}

void Dict::_dictRehashStep()
{
	dictRehash(1);
}

/* Expand the hash table if needed */
int Dict::_dictExpandIfNeeded()
{
/* Incremental rehashing already in progress. Return. */
    if (dictIsRehashing()) return DICT_OK;

    /* If the hash table is empty expand it to the initial size. */
    if (_ht[0].size == 0) return dictExpand(HT_INIT_SIZE);

    if (_ht[0].used >= _ht[0].size &&
        (CAN_RESIZE || _ht[0].used / _ht[0].size > FORCE_RESIZE_RETIO))
    {
        return dictExpand(_ht[0].used*2);
    }

    return DICT_OK;
}

unsigned long Dict::_dictNextSize(unsigned long size)
{
	unsigned long i = HT_INIT_SIZE;
	while (i < size) {
		i *= 2;
	}
	return i;
}

unsigned int Dict::_dictGenHashFunction(const void *key, int len) 
{
    /* 'm' and 'r' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.  */
    unsigned int seed = dict_hash_function_seed;
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */
    unsigned int h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */
    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        unsigned int k = *(unsigned int*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated. */
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

