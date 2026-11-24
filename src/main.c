#include "app.h"
#include "error.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    int status;

    /* This is a message string that can be accessed by the whole game to
     * to capture errors */
    char* message = malloc(256 * sizeof(char));

    if (message == NULL) {
        return -1;
    }

    str[0] = '\0';

    /* Gameinfo loads the basic gameplay and assets and makes a handle
     * for save if lethal failure */
    GameInfo Info;

    status = app_startup(&Info, message);
    if (status) {
        app_err(status, message);
        free(message);
        app_shutdown(&Info);
    }

    status = run_app(argc, argv, &Info, message);
    if (status) {
        app_err(status, message);
        free(message);
        app_shutdown(&Info);
    }

    free(message);
    app_shutdown(&Info);

    return 0;
}
