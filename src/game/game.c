/* This is where all of the gamestate stuff goes */
#include "game.h"
#include "engine/engine.h"

int run_game(GameInfo* info, char* message)
{
    int status;

    /* main loop */
    status = run_engine();

    return status;
}

