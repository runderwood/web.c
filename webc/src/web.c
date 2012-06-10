#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
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
        //webapp_log(app, "request!");
        htreq* r = htreq_new();
        htreq_init(r);
        //if(r == NULL) printf("r is null.\n");
        //else printf("uri: %d\n", strlen(r->uri));
        htreq_del(r);
    }
    return 0;
}

htreq* htreq_new() {
    htreq* req = malloc(sizeof(htreq));
    return req;
}

int htreq_init(htreq* r) {
    const char *htmethod = getenv("REQUEST_METHOD");
    assert(htmethod);
    HTMETHODVAL(htmethod, r->method);
    const char* uri = getenv("PATH_INFO");
    assert(uri);
    int uri_len = -1;
    if(uri != NULL) {
        uri_len = strlen(uri);
    }
    if(uri_len >= 0) {
        r->uri = malloc(uri_len+1);
        strcpy(r->uri, uri);
        r->uri[uri_len] = '\0';
    }
    //char* rawqs = "aaa=one+and+two+and+three&hello=one&hello=two&byebye=three&hello=third%40&hello=fourth+whoa";
    //qslist* qs = qsparse(rawqs, strlen(rawqs));
    /*qsval* v = qslist_get(qs, "hello");
    if(v) {
        lnode* n = v->vals->head;
        while(n) {
            fprintf(stderr, "%s:%s\n", v->key, (char*)n->cargo);
            n = n->next;
        }
    } else fprintf(stderr, "no 'hello' val");*/
    //fprintf(stderr, "get_one(hello): %s\n", (char*)qslist_get_one(qs, "hello"));
    //qslist_del(qs);
    return 0;
}

void htreq_del(htreq* r) {
    if(r->uri != NULL) free(r->uri);
    r->uri = NULL;
    free(r);
    return;
}
