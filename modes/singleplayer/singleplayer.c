#include "../../auto_import.h"

void playWithBotMode() {
    //    player1
    int selectUserOrder = getUserMenuOrder(&selectUserMenuShower, 0);
    int putShipOrder = getUserMenuOrder(&selectPutShipMenuShower, 0);
    Player player = loadPlayerFromUserChoice(selectUserOrder, putShipOrder);
    if (isApplicationInProduction) {
        printf("sleep for 5 seconds and then clear screen . . . \n");
        sleep(5);
        clearScreen();
    }
//    bot
    Player bot = setUpBot();
    if (isApplicationInProduction) {
        printf("sleep for 3 seconds and then clear screen . . . \n");
        sleep(3);
        clearScreen();
    }

//    start game with Bot
    startGameWithBot(player, bot);
}

void startGameWithBot(Player player, Player bot) {
    fillMapWithLetter(player.attackMap, 'N');
    fillMapWithLetter(bot.attackMap, 'N');
    player.isPlayerTurn = true;

    while (!thereIsWinner(player, bot)) {
        Player *attacker = findAttacker(&player, &bot);
        Player *defender = findDefender(&player, &bot);

        if (strcmp(player.userData.name, attacker->userData.name) == 0) {
//            player is the attacker
            printf("%s score : %d\n", attacker->userData.name, attacker->userData.score);
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
                if (isApplicationInProduction) {
                    clearScreen();
                }
                return;
            }
        } else {
//            it is bot turn
            Coordinate botAttackCoordinate = handleBotMove(attacker, defender);
            printf("bot attacked coordinate with x: %d, y: %d\n", botAttackCoordinate.x, botAttackCoordinate.y);
            if (isApplicationInProduction) {
                printf("sleep for 2 seconds and then clear screen . . . \n");
                sleep(2);
                clearScreen();
            }
        }
    }

    Player *winner = findWinner(&player, &bot);
    Player *loser = findLoser(&player, &bot);

    printf("game is finished :)");
    printf("%s has won the game :)", winner->userData.name);

    updatePlayerData(winner, loser);
    updateUserScoreBoard(player.userData);

    if (isApplicationInProduction) {
        printf("sleep for 3 seconds and then clear screen . . . \n");
        sleep(3);
        clearScreen();
    }
}

Coordinate handleBotMove(Player *attacker, Player *defender) {
    Coordinate attackCoordinate = generateRandomCoordination();

    while (!isValidAttackLocation(attacker->attackMap, attackCoordinate)) {
        attackCoordinate = generateRandomCoordination();
    }

    botAttack(attackCoordinate, attacker, defender);
    return attackCoordinate;
}

void botAttack(Coordinate coordination, Player *attacker, Player *defender) {
    if (!itIsAHit(coordination, *defender)) {
//        change turns
        printf("bot didn't hit\n");
        attacker->isPlayerTurn = false;
        defender->isPlayerTurn = true;
        return;
    }
    printf("bot hit\n");
//    it hits the target
    Ship *damagedShip = findDamagedShip(coordination, *defender);
    (damagedShip->hitCount)++;

    if (shipHasSunk(damagedShip)) {
        handleShipDelete(defender->shipHead, damagedShip);
        return;
    }
}

Player setUpBot() {
    User user;
//    setting up user data for bot
    user.id = -1;
    strcpy(user.name, "BOT");
    user.winLoseRatio = 0;
    user.loseCount = 0;
    user.winCount = 0;
    user.score = 0;

    //  setting up bot player ships
    return handleAutoShipPlacement(user);
}
