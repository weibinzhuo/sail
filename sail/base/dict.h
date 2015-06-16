
#ifndef _SAIL_DICT_H
#define _SAIL_DICT_H

#define DICT_NOTUSED(V) ((void) V)

enum DICTRET
{
	DICT_OK = 0,
	DICT_ERR,
};

struct DictEntry
{
    void *key;
    void *val;
    struct DictEntry *next;
};

struct Dictht
{
    struct DictEntry **table;
    unsigned long size;
    unsigned long sizemark;
    unsigned long used;
};


class Dict
{
public:
    Dict();
    virtual ~Dict();

    virtual int dictHashKey(const void *key);
    virtual int dictCompareKeys(const void *key1, const void *key2);

    DictEntry* dictFind(const void *key);
    int dictAdd(const void *key, const void *val);
    int dictReplace(const void *key, const void *val);
    int dictDelete(const void *key);
    int dictDeleteNoFree(const void *key);

    bool dictIsRehashing() const { return _rehashidx != -1; }
    int dictRehash(int n);
    int dictExpand(unsigned long size);

private:
    void _dictRelease();
    DictEntry *_dictAddRaw(const void *key);
    int _dictKeyIndex(const void *key);
    int _dictGenericDelete(const void *key, int nofree);
    void _dictRehashStep();
    int _dictExpandIfNeeded();

private:
    static void _dictReset(Dictht *ht);
    static int _dictClear(Dictht *ht);
    static void _dictSetKey(DictEntry *entry, const void *key) { entry->key = const_cast<void *>(key); }
    static void _dictSetVal(DictEntry *entry, const void *val) { entry->val = const_cast<void *>(val); }
    static void _dictFreeKey(DictEntry *entry) { DICT_NOTUSED(entry); }
    static void _dictFreeVal(DictEntry *entry) { DICT_NOTUSED(entry); }

	static unsigned long _dictNextSize(unsigned long size);
    static unsigned int _dictGenHashFunction(const void *key, int len);

	static const int CAN_RESIZE = 1;
	static const unsigned int FORCE_RESIZE_RETIO = 5;
	static const int HT_INIT_SIZE = 4;

private:
    Dictht _ht[2];
    int _rehashidx;
};

#endif /* _SAIL_DICT_H */


