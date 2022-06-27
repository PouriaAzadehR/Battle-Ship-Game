#ifndef SEA_BATTLE_PLAYER_DEFINE_H
#define SEA_BATTLE_PLAYER_DEFINE_H

#include "../../auto_import.h"

typedef struct {
    User userData;
    Ship *shipHead;
    char attackMap[10][10];
    bool isPlayerTurn;
} Player;

#endif //SEA_BATTLE_PLAYER_DEFINE_H
