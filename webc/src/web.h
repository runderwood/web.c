#ifndef WEB_H
#define WEB_H
#include "list.h"

typedef struct appconf_s {
    char** routes;
    int log_level;
} appconf;

typedef struct webapp_s {
    int served;
    int log_level;
} webapp;

webapp* webapp_new();
int webapp_init(webapp* app, appconf* config);
int webapp_serve(webapp* app);
void webapp_del(webapp* app);
void webapp_log(webapp* app, const char* msg, ...);

typedef struct htreq_s {
    char* method;
    char* uri;
    char* body;
} htreq;

htreq* htreq_new();
int htreq_init(htreq* req, char** env);
void htreq_del(htreq* r);

typedef struct htresp_s {
    llist* headers;
    char* body;
} htresp;

htresp* htresp_new();
int htresp_print(htresp* r);
void htresp_del(htresp* resp);

// function pointer types for handler, etc.

#endif
