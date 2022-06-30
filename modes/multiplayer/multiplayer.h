#ifndef SEA_BATTLE_MULTIPLAYER_H
#define SEA_BATTLE_MULTIPLAYER_H
Player *findWinner(Player *player1, Player *player2);

void updateUserScoreBoard(User user);

void updatePlayerData(Player *winner, Player *loser);

Player *findLoser(Player *player1, Player *player2);


void handleMakeMove(Player *attacker, Player *defender);

bool isValidAttackLocation(char attackMap[10][10], Coordinate coordination);

bool isCoordinationOutOfRange(Coordinate coordination);

void attack(Coordinate coordination, Player *attacker, Player *defender);

void updateMapWithLetter(char map[10][10], Coordinate coordination, char letter);

bool itIsAHit(Coordinate coordination, Player defender);

bool shipIsHit(Ship *ship, Coordinate coordination);

Ship *findDamagedShip(Coordinate coordination, Player defender);

bool shipHasSunk(Ship *ship);

void handleShipDelete(Ship *head, Ship *deleteTarget);

Ship *deleteFromBeginning(Ship *head);

Ship *deleteFromEnd(Ship *head);

Ship *deleteFromMiddle(Ship *head, Ship *deleteTarget);

Ship *findBeforeNode(Ship *head, Ship *node);

void updateMapForShipHit(char map[10][10], Coordinate coordination);

void updateMapForShipSunk(char map[10][10], Ship *sunkShip);

void updateAttackerScore(Player *attacker, int shipWidth, bool isShipSunk);

void playWithFriendMode();

void startGameWithFriend(Player player1, Player player2);

bool thereIsWinner(Player player1, Player player2);

Player *findAttacker(Player *player1, Player *player2);

Player *findDefender(Player *player1, Player *player2);

bool fileDoesNotContainUser(FILE *fileHandler, User user);

#endif //SEA_BATTLE_MULTIPLAYER_H
