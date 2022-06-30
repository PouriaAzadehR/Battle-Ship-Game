#include "../../auto_import.h"


void playWithFriendMode() {
//    player1
    int selectUserOrder = getUserMenuOrder(&selectUserMenuShower, 1);
    int putShipOrder = getUserMenuOrder(&selectPutShipMenuShower, 1);
    Player player1 = loadPlayerFromUserChoice(selectUserOrder, putShipOrder);
    if (isApplicationInProduction) {
        printf("after 5 seconds console will be cleared.\n");
        sleep(5);
        clearScreen();
    }

//    player2
    selectUserOrder = getUserMenuOrder(&selectUserMenuShower, 2);
    putShipOrder = getUserMenuOrder(&selectPutShipMenuShower, 2);
    Player player2 = loadPlayerFromUserChoice(selectUserOrder, putShipOrder);
    if (isApplicationInProduction) {
        printf("after 5 seconds console will be cleared.\n");
        sleep(5);
        clearScreen();
    }
//    starting the game
    startGameWithFriend(player1, player2);
}

Player *findWinner(Player *player1, Player *player2) {
    return player1->shipHead ? player1 : player2;
}


Player *findLoser(Player *player1, Player *player2) {
    return player1->shipHead ? player2 : player1;
}


void updatePlayerData(Player *winner, Player *loser) {
    // updating winner data
    (winner->userData.winCount)++;
    winner->userData.winLoseRatio = winner->userData.winCount / (double) winner->userData.loseCount;
    winner->isPlayerTurn = false;

    (loser->userData.loseCount)++;
    loser->userData.winLoseRatio = loser->userData.winCount / (double) loser->userData.loseCount;
    loser->isPlayerTurn = false;
}


bool fileDoesNotContainUser(FILE *fileHandler, User user) {
    User iterationUser;
    do {
        fread(&iterationUser, sizeof(user), 1, fileHandler);
        if (strcmp(iterationUser.name, user.name) == 0) {
//            go to beginning of the file
            fseek(fileHandler, 0, SEEK_SET);
            return true;
        }
    } while (feof(fileHandler));

//    go to beginning of the file
    fseek(fileHandler, 0, SEEK_SET);
    return false;
}


void updateUserScoreBoard(User user) {
    FILE *scoreboardDatabaseHandler = fopen("../database/users.database.binary", "ab+");
    fseek(scoreboardDatabaseHandler, 0, SEEK_SET);


//    make a temp file
    FILE *tempUsersDatabase = fopen("../database/users.temp.database.binary", "wb");

//    add all other data to temp file
    User iterationUser;
    do {
        fread(&iterationUser, sizeof(User), 1, scoreboardDatabaseHandler);

        if (strcmp(iterationUser.name, user.name) != 0) {
            fwrite(&iterationUser, sizeof(iterationUser), 1, tempUsersDatabase);
        } else {
            fwrite(&user, sizeof(user), 1, tempUsersDatabase);
        }
    } while (!feof(scoreboardDatabaseHandler));

//    remove old file
    remove("../database/users.database.binary");

//    rename file to the actual file name
    rename("../database/users.temp.database.binary", "../database/users.database.binary");
}

void startGameWithFriend(Player player1, Player player2) {
    fillMapWithLetter(player1.attackMap, 'N');
    fillMapWithLetter(player2.attackMap, 'N');
    player1.isPlayerTurn = true;


    while (!thereIsWinner(player1, player2)) {
        Player *attacker = findAttacker(&player1, &player2);
        Player *defender = findDefender(&player1, &player2);

        printf("%s score: %d\n", attacker->userData.name, attacker->userData.score);
        printf("%s score: %d\n", defender->userData.name, defender->userData.score);

        printf("%s turn:\n", attacker->userData.name);
        int order = getUserMenuOrder(&inGameMenuShower, 0);

        if (order == 1) {
            handleMakeMove(attacker, defender);

            if (isApplicationInProduction) {
                printf("sleep for 2 seconds and then clear screen . . . \n");
                sleep(2);
                clearScreen();
            }

        } else {
            handleGameSave(attacker, defender);
            printf("game saved successfully!\n");
            if (isApplicationInProduction) {
                clearScreen();
            }
            return;
        }
    }

//  update users data in score board
    Player *winner = findWinner(&player1, &player2);
    Player *loser = findLoser(&player1, &player2);

    printf("game is finished :)");
    printf("%s has won the game :)", winner->userData.name);

    updatePlayerData(winner, loser);

    updateUserScoreBoard(winner->userData);
    updateUserScoreBoard(loser->userData);

    if (isApplicationInProduction) {
        printf("sleep for 2 seconds and then clear the screen\n");
        sleep(2);
        clearScreen();
    }
}


void handleMakeMove(Player *attacker, Player *defender) {
    showMap(attacker->attackMap);

    Coordinate attackCoordination;
    printf("please input attack coordination X:\t");
    scanf("%d", &(attackCoordination.x));
    printf("please input attack coordination Y:\t");
    scanf("%d", &(attackCoordination.y));

    while (!isValidAttackLocation(attacker->attackMap, attackCoordination)) {
        showMap(attacker->attackMap);

        printf("NOT A VALID LOCATION\n");

        printf("please input attack coordination X:\t");
        scanf("%d", &(attackCoordination.x));
        printf("please input attack coordination Y:\t");
        scanf("%d", &(attackCoordination.y));
    }

    attack(attackCoordination, attacker, defender);
}

void attack(Coordinate coordination, Player *attacker, Player *defender) {
    if (!itIsAHit(coordination, *defender)) {
        printf("NO HIT\n");
//        update the map
        updateMapWithLetter(attacker->attackMap, coordination, 'W');

//        change turns
        attacker->isPlayerTurn = false;
        defender->isPlayerTurn = true;
        return;
    }

    printf("IT IS A HIT!\n");
    Ship *damagedShip = findDamagedShip(coordination, *defender);
    (damagedShip->hitCount)++;


    if (shipHasSunk(damagedShip)) {
        updateMapForShipSunk(attacker->attackMap, damagedShip);
        updateAttackerScore(attacker, damagedShip->width, true);
        handleShipDelete(defender->shipHead, damagedShip);
        return;
    }

    updateAttackerScore(attacker, damagedShip->width, false);
    updateMapForShipHit(attacker->attackMap, coordination);
}

void updateAttackerScore(Player *attacker, int shipWidth, bool isShipSunk) {
    if (!isShipSunk) {
        (attacker->userData.score)++;
        return;
    }

    switch (shipWidth) {
        case 5:
            attacker->userData.score += 6;
            break;
        case 3:
            attacker->userData.score += 9;
            break;
        case 2:
            attacker->userData.score += 13;
            break;
        case 1:
            attacker->userData.score += 26;
    }
}

void updateMapForShipSunk(char map[10][10], Ship *sunkShip) {
    if (shipIsHorizontal(sunkShip->start, sunkShip->end)) {
        int y = sunkShip->start.y;

        int lowestX = findLowestX(sunkShip->start, sunkShip->end);
        int highestX = findHighestX(sunkShip->start, sunkShip->end);

        for (int row = y - 1; row <= y + 1; ++row) {
            if (row == -1) {
                row = 0;
            }

            if (row == 10) {
                break;
            }
            for (int column = lowestX - 1; column <= highestX + 1; ++column) {
                if (column == -1) {
                    column = 0;
                }

                if (column == 10) {
                    break;
                }
                map[row][column] = 'C';
            }
        }
    } else {
        int x = sunkShip->start.x;

        int lowestY = findLowestY(sunkShip->start, sunkShip->end);
        int highestY = findHighestY(sunkShip->start, sunkShip->end);

        for (int row = lowestY - 1; row <= lowestY + 1; ++row) {
            if (row == -1) {
                row = 0;
            }

            if (row == 10) {
                break;
            }
            for (int column = x - 1; column <= x + 1; ++column) {
                if (column == -1) {
                    column = 0;
                }

                if (column == 10) {
                    break;
                }
                map[row][column] = 'C';
            }
        }
    }
}

void updateMapForShipHit(char map[10][10], Coordinate coordination) {
    map[coordination.y][coordination.x] = 'E';
}

void handleShipDelete(Ship *head, Ship *deleteTarget) {
    Ship *tale = findTale(head);

    if (deleteTarget == head) {
        head = deleteFromBeginning(head);
    } else if (deleteTarget == tale) {
        head = deleteFromEnd(head);
    } else {
        head = deleteFromMiddle(head, deleteTarget);
    }
}

Ship *deleteFromMiddle(Ship *head, Ship *deleteTarget) {
    Ship *beforeNode = findBeforeNode(head, deleteTarget);

    beforeNode->next = deleteTarget->next;

    deleteTarget->next = NULL;
    free(deleteTarget);

    return head;
}

Ship *findBeforeNode(Ship *head, Ship *node) {
    Ship *iterationNode = head;

    while (iterationNode->next != node) {
        iterationNode = iterationNode->next;
    }

    return iterationNode;
}

Ship *deleteFromEnd(Ship *head) {
    Ship *oldTale = findTale(head);
    Ship *newTale = findBeforeNode(head, oldTale);

    newTale->next = NULL;

    free(oldTale);
    return head;
}

Ship *deleteFromBeginning(Ship *head) {
    Ship *newHead = head->next;

    head->next = NULL;
    free(head);

    return newHead;
}

bool shipHasSunk(Ship *ship) {
    return ship->hitCount == ship->width;
}

Ship *findDamagedShip(Coordinate coordination, Player defender) {
    Ship *iterationShip = defender.shipHead;

    while (iterationShip) {
        if (shipIsHit(iterationShip, coordination)) {
            return iterationShip;
        }
        iterationShip = iterationShip->next;
    }
}

bool shipIsHit(Ship *ship, Coordinate coordination) {
    if (shipIsHorizontal(ship->start, ship->end)) {
        int lowestX = findLowestX(ship->start, ship->end);
        int highestX = findHighestX(ship->start, ship->end);

        int shipY = ship->start.y;

        return (shipY == coordination.y) && (lowestX <= coordination.x) && (coordination.x <= highestX);
    } else {
        int lowestY = findLowestY(ship->start, ship->end);
        int highestY = findHighestY(ship->start, ship->end);

        int shipX = ship->start.x;

        return (shipX == coordination.x) && (lowestY <= coordination.y) && (coordination.y <= highestY);
    }
}

bool itIsAHit(Coordinate coordination, Player defender) {
    Ship *iterationShip = defender.shipHead;

    while (iterationShip) {
        if (shipIsHit(iterationShip, coordination)) {
            return true;
        }

        iterationShip = iterationShip->next;
    }

    return false;
}

void updateMapWithLetter(char map[10][10], Coordinate coordination, char letter) {
    map[coordination.y][coordination.x] = letter;
}

bool isValidAttackLocation(char attackMap[10][10], Coordinate coordination) {
    return attackMap[coordination.y][coordination.x] == 'N' && isCoordinationOutOfRange(coordination);
}

Player *findDefender(Player *player1, Player *player2) {
    return player1->isPlayerTurn ? player2 : player1;
}

Player *findAttacker(Player *player1, Player *player2) {
    return player1->isPlayerTurn ? player1 : player2;
}

bool thereIsWinner(Player player1, Player player2) {
    return ((player1.shipHead == NULL) || (player2.shipHead == NULL));
}
