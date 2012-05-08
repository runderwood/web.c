#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcgi_stdio.h>
#include <string.h>
#include "web.h"
#include "url.h"

extern char** environ;

webapp* webapp_new() {
    webapp* app = malloc(sizeof(webapp));
    return app;
}

void webapp_del(webapp* app) {
    free(app);
    return;
}

int webapp_init(webapp* app, appconf* conf) {
    // configure and init here.
    return 0;
}

void webapp_log(webapp* app, const char* fmt, ...) {
    va_list params;
    va_start(params, fmt);
    vfprintf(stderr, fmt, params);
    fprintf(stderr, "\n");
    va_end(params);
    return;
}

int webapp_serve(webapp* app) {
    while(FCGI_Accept() >= 0) {
        webapp_log(app, "request!");
        htreq* r = htreq_new();
        htreq_init(r, environ);
        htreq_del(r);
    }
    return 0;
}

htreq* htreq_new() {
    htreq* req = malloc(sizeof(htreq));
    return req;
}

int htreq_init(htreq* r, char** envp) {
    //for(;*envp != NULL; envp++) {
    //    fprintf(stderr, "%s\n", *envp);
    //}
    char* rawqs = "aaa=one+and+two+and+three&hello=one&hello=two&byebye=three&hello=third%40&hello=fourth+whoa";
    qslist* qs = qsparse(rawqs, strlen(rawqs));
    /*qsval* v = qslist_get(qs, "hello");
    if(v) {
        lnode* n = v->vals->head;
        while(n) {
            fprintf(stderr, "%s:%s\n", v->key, (char*)n->cargo);
            n = n->next;
        }
    } else fprintf(stderr, "no 'hello' val");*/
    //fprintf(stderr, "get_one(hello): %s\n", (char*)qslist_get_one(qs, "hello"));
    qslist_del(qs);
    return 0;
}

void htreq_del(htreq* r) {
    free(r);
    return;
}
