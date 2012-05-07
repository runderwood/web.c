#include <stdlib.h>
#include "web.h"

int main(int argc, char** argv) {

    appconf config;

    webapp app;

    webapp_init(&app, &config);

    webapp_serve(&app);
    
    return EXIT_SUCCESS;
}
