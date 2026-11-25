#include "app.h"
#include "error.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    int status;

    /* This is a message string that can be accessed by the whole game to
     * to capture errors */
    char* message = malloc(ERR_MSG_LEN_MAX + 1 * sizeof(char));

    if (message == NULL) {
        return -1;
    }

    message[0] = '\0';
    message[ERR_MSG_LEN_MAX] = '\0';

    /* Gameinfo loads the basic gameplay and assets and makes a handle
     * for save if lethal failure */
    GameInfo info;

    status = app_startup(&info, message);
    if (status) {
        app_err(status, message);
        free(message);
        app_shutdown(&info);
    }

    status = run_app(argc, argv, &info, message);
    if (status) {
        app_err(status, message);
        free(message);
        app_shutdown(&info);
    }

    free(message);
    app_shutdown(&info);

    return 0;
}
