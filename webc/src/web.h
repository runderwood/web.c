#ifndef WEB_H
#define WEB_H
#include "list.h"

#define HT_GET 1
#define HT_POST 2
#define HT_PUT 4
#define HT_DELETE 8
#define HT_HEAD 16

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
    int method;
    char* uri;
    char* body;
} htreq;

htreq* htreq_new();
int htreq_init(htreq* req);
void htreq_del(htreq* r);

typedef struct htresp_s {
    llist* headers;
    char* body;
} htresp;

htresp* htresp_new();
int htresp_print(htresp* r);
void htresp_del(htresp* resp);

// function pointer types for handler, etc.
typedef int (*reqhandler)(htreq* req);

#define HTMETHODVAL(sval, ival) if(sval != NULL) {\
    if(!strcmp(sval, "GET")) {\
        ival = HT_GET;\
    } else if(!strcmp(htmethod, "POST")) {\
        ival = HT_POST;\
    } else if(!strcmp(htmethod, "PUT")) {\
        ival = HT_PUT;\
    } else if(!strcmp(htmethod, "DELETE")) {\
        ival = HT_DELETE;\
    } else if(!strcmp(htmethod, "HEAD")) {\
        ival = HT_HEAD;\
    } else {\
        ival = -1;\
    }\
} else ival = -2;

#define HTMETHODSTR(ival, sval) if(ival > 0) {\
    if(ival == HT_GET) {\
        strcpy(sval, "GET");\
        sval[3] = '\0';\
    } else if(ival == HT_POST) {\
        strcpy(sval, "POST");\
        sval[4] = '\0';\
    } else if(ival == HT_PUT) {\
        strcpy(sval, "PUT");\
        sval[3] = '\0';\
    } else if(ival == HT_DELETE) {\
        strcpy(sval, "DELETE");\
        sval[6] = '\0';\
    } else if(ival == HT_HEAD) {\
        strcpy(sval, "HEAD");\
        sval[4] = '\0';\
    } else {\
        sval[0] = '\0';\
    }\
} else sval[0] = '\0';

#endif
