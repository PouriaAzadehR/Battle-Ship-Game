#ifndef SEA_BATTLE_SINGLEPLAYER_H
#define SEA_BATTLE_SINGLEPLAYER_H

void playWithBotMode();

Player setUpBot();

void startGameWithBot(Player player, Player bot);

Coordinate handleBotMove(Player *attacker, Player *defender);

void botAttack(Coordinate coordination, Player *attacker, Player *defender);
#endif //SEA_BATTLE_SINGLEPLAYER_H
