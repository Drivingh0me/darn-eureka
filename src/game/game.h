#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

typedef struct {
    int info;
} GameInfo;

int run_game(GameInfo* info, char* message);

#endif /* GAME_ENGINE_H */
