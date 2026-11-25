#ifndef APP_H
#define APP_H

#include "error.h"
#include "game/game.h"

int app_startup(GameInfo *info, char* message);
int run_app(int argc, char **argv, GameInfo *info, char* message);
void app_shutdown(GameInfo *info);

#endif /* APP_H */
