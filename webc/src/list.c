#include <stdlib.h>
#include <assert.h>
#include "list.h"

llist* list_new(lnode_free_func node_free) {
    llist* l = malloc(sizeof(llist));
    assert(l != NULL);
    l->node_free = node_free;
    list_init(l);
    return l;
}

void list_del(llist* l) {
    void* popped;
    while(l->len > 0) {
        popped = list_pop(l);
        assert(popped);
        l->node_free(popped);
    }
    free(l);
    return;
}

int list_init(llist* l) {
    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
    return 0;
}

int list_len(llist* l) {
    return l->len;
}

int list_push(llist *l, void* c) {
    lnode* n = malloc(sizeof(lnode));
    assert(n != NULL);
    n->cargo = c;
    if(l->head == NULL) {
        l->head = n;
        l->tail = n;
    }
    l->tail->next = n;
    n->prev = l->tail;
    n->next = NULL;
    l->tail = n;
    l->len++;
    return l->len;
}

void* list_pop(llist* l) {
    if(l->head == NULL) {
        return NULL;
    }
    lnode* t = l->tail;
    l->tail = t->prev;
    l->tail->next = NULL;
    void* c = t->cargo;
    free(t);
    l->len--;
    return c;
}
