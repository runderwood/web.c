#ifndef DICT_H
#define DICT_H

#include <stdint.h>

#define DICT_MINSZ 8

typedef struct dict_entry_s {
    void* k;
    size_t ksz;
    void* v;
    size_t vsz;
} dict_entry;

typedef uint8_t kcmp_fxn(const void *ka, size_t asz, const void* kb, size_t bsz);
typedef uint64_t khash_fxn(const void* k, size_t ksz);

typedef struct dict_s {
    size_t cabsz;
    size_t mask;
    size_t empty;
    dict_entry** cab;
    kcmp_fxn* kcmp;
    khash_fxn* hash; 
} dict;

uint64_t djb_hash(const void* k, const size_t ksz);
dict_entry* dict_entry_new(void);
dict_entry* dict_entry_new4(const void* k, size_t ksz, const void* v, size_t vsz);
void dict_entry_del(dict_entry* e);
dict* dict_new(void);
void dict_del(dict* d);
int dict_put(dict* d, const void* k, size_t ksz, const void* v, size_t vsz, size_t idx, int repl);
int dict_put4(dict* d, dict_entry* e, size_t idx, int repl);
void* dict_get(dict* d, const void* k, size_t ksz, size_t* vsz);
int dict_set(dict* d, const void* k, size_t ksz, const void* v, size_t vsz);
size_t dict_resize(dict* d, int mult);

char* dict_ser(dict* d, size_t* sz);
dict* dict_unser(const char* buf, size_t bufsz);

#endif
