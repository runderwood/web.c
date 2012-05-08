#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "url.h"
#include "util.h"
#include "list.h"

#include <stdio.h>

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

qslist* qsparse(const char* qstr, int len) {
    qslist* qs = qslist_new();
    int i;
    char* kbuf = malloc(len);
    assert(kbuf != NULL);
    char* vbuf = malloc(len);
    assert(vbuf != NULL);
    int klen, vlen, declen;
    char c;
    char* kdec = NULL;
    char* vdec = NULL;
    for(i=0, klen=-1, vlen=-1; i<=len; i++) {
        if(i < len) {
            c = *(qstr+i);
        } else {
            c = '\0';
        }
        if(klen < 0 && vlen < 0) {
            klen = 0;
        }
        if(vlen < 0) {
            if(c == '&' || c == '\0') {
                *(kbuf+klen) = '\0';
                kdec = urldecode(kbuf, klen, &declen);
                qslist_val_add(qs, kdec, "");
                free(kdec);
                kdec = NULL;
                klen = -1;
                vlen = -1;
            } else if(c == '=') {
                *(kbuf+klen) = '\0';
                vlen = 0;
            } else {
                *(kbuf+klen++) = c;
            }
        } else {
            if(c == '&' || c == '\0') {
                *(vbuf+vlen) = '\0';
                kdec = urldecode(kbuf, klen, &declen);
                vdec = urldecode(vbuf, vlen, &declen);
                qslist_val_add(qs, kdec, vdec);
                free(kdec);
                free(vdec);
                kdec = vdec = NULL;
                klen = -1;
                vlen = -1;
            } else {
                *(vbuf+vlen++) = c;
            }
        }
    }
    return qs;
}

static char* hexchars = "0123456789ABCDEF";

char* urlencode(const char* str, int len, int* enclen) {
    char* encstr = malloc(sizeof(char)*3*len+1);
    assert(encstr != NULL);
    char c;
    int i,j;
    for(i=0, j=0; i<len; i++, j++) {
        c = str[i];
        if(c == ' ') {
            *(encstr+j) = '+';
        } else if((c < '0' && c != '.' && c != '-') ||
            (c < 'A' && c > '9') ||
            (c > 'Z' && c < 'a' && c != '_') ||
            (c > 'z' && c != '~')) {
            *(encstr+j++) = '%';
            *(encstr+j++) = hexchars[c >> 4];
            *(encstr+j) = hexchars[c & 15];
            *(encstr+j+1) = '\0';
        } else {
            *(encstr+j) = c;
        }
    }
    *(encstr+j+1) = '\0';
    *enclen = j;
    return encstr;
}

char* urldecode(const char* str, int len, int* declen) {
    char* decstr = malloc(len+1);
    *declen = 0;
    char* src = (char*)str;
    assert(decstr != NULL);
    char c;
    int i = 0;
    int so, o;
    for(o=0, so=0; so<len; so++, o++) {
        c = *(src+so);
        if(c == '%' && len-so > 2 && isxdigit((int) *(src+so+1)) &&
            isxdigit((int)*(src+so+2))) {
            i = xtoi(*(src+so+1));
            i = i << 4;
            i += xtoi(*(src+so+2));
            *(decstr+o) = (char)i;
            so += 2;
        } else if(c == '+') {
            *(decstr+o) = ' ';
        } else {
            *(decstr+o) = c;
        }
    }
    *(decstr+o) = '\0';
    *declen = o;
    return decstr;
}
