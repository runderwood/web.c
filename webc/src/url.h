#ifndef URL_H
#define URL_H

#include "list.h"

typedef struct qsval_s {
    char* key;
    llist* vals;
} qsval;

qsval* qsval_new(const char* k);
void qsval_del(qsval* qsv);
int qsval_key_set(qsval* qsv, const char* k);
const char* qsval_key_get(qsval* qsv);
int qsval_val_add(qsval* qsv, const char* val);
int qsval_val_num(qsval* qsv);

typedef struct qslist_s {
    llist* list;
} qslist;

qslist* qslist_new(void);
void qslist_del(qslist* qs);
qsval* qslist_get(qslist* qs, const char* k);
const char* qslist_get_one(qslist* qs, const char* k);
int qslist_val_add(qslist* qs, const char* k, const char* v);

qslist* qsparse(const char* qstr);

#endif
