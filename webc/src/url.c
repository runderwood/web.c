#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "url.h"
#include "list.h"

int qsval_key_set(qsval* qsv, const char* k) {
    int len = strlen(k);
    if(!qsv->key || strlen(qsv->key) < len) {
        qsv->key = realloc(qsv->key, len+1);
    }
    memcpy(qsv->key, k, len);
    *(qsv->key+len) = '\0';
    return len;
}

qsval* qsval_new(const char* k) {
    qsval* qsv = malloc(sizeof(qsval));
    assert(qsv != NULL);
    qsv->key = NULL;
    qsval_key_set(qsv, k);
    qsv->vals = list_new(free);
    return qsv;
}

void qsval_del(qsval* qsv) {
    free(qsv->key);
    list_del(qsv->vals);
    free(qsv);
    return;
}

const char* qsval_key_get(qsval* qsv) {
    return (const char*)qsv->key;
}

int qsval_val_num(qsval* qsv) {
    return list_len(qsv->vals);
}

int qsval_val_add(qsval* qsv, const char* v) {
    int vlen = strlen(v);
    char* vc = malloc(vlen+1);
    assert(vc != NULL);
    memcpy(vc, v, vlen+1);
    list_push(qsv->vals, vc);
    return qsval_val_num(qsv);
}

qslist* qslist_new(void) {
    qslist* qs = malloc(sizeof(qslist));
    assert(qs != NULL);
    qs->list = list_new(qsval_del);
    return qs;
}

void qslist_del(qslist* qs) {
    list_del(qs->list);
    free(qs);
    return;
}

qsval* qslist_get(qslist* qs, const char* k) {
    qsval* qsv;
    assert(qs && qs->list);
    lnode* n = (qs->list)->head;
    while(n != NULL) {
        qsv = (qsval*)n->cargo;
        if(strcmp(k, qsv->key) == 0) {
            return qsv;
        }
        n = n->next;
    }
    return NULL;
}

const char* qslist_get_one(qslist* qs, const char* k) {
    qsval* qsv = qslist_get(qs, k);
    const char* v = NULL;
    if(qsv != NULL) {
        v = (const char*)(qsv->vals->tail->cargo);
    }
    return v;
}

int qslist_val_add(qslist* qs, const char* k, const char* v) {
    qsval* qsv = qslist_get(qs, k);
    if(qsv == NULL) {
        qsv = qsval_new(k);
        list_push(qs->list, qsv);
    }
    return qsval_val_add(qsv, v);
}

qslist* qsparse(const char* qstr) {
    qslist* qs = qslist_new();
    qslist_val_add(qs, "hello", "world");
    qslist_val_add(qs, "hello", "WORLD");
    qslist_val_add(qs, "byeby", "WOrld");
    return qs;
}
