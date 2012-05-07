#ifndef LIST_H
#define LIST_H
typedef struct lnode_s {
    struct lnode_s* prev;
    struct lnode_s* next;
    void* cargo;
} lnode;

typedef void (*lnode_free_func)();

typedef struct llist_s {
    lnode* head;
    lnode* tail;
    int len;
    lnode_free_func node_free;
} llist;

llist* list_new();
void list_del(llist* l);
int list_init(llist* l);
int list_push(llist* l, void* d);
void* list_pop(llist* l);
int list_len(llist* l);
#endif
