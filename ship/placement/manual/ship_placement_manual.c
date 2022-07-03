#include "../../../auto_import.h"

Player handleManualShipPlacement(User user) {
    Player player;
    player.userData = user;

    player.isPlayerTurn = false;

    return putShipsManually(player);
}

Player putShipsManually(Player player) {
    Ship *shipHead = NULL;
    char map[10][10];
    fillMapWithLetter(map, 'N');


    int shipWidth = 5;
    int shipCount = 1;

    while (shipWidth >= 1) {
        if (shipWidth == 4) {
            shipWidth--;
            continue;
        }

        shipHead = putShip(shipHead, shipCount, shipWidth, map);
        shipWidth--;
        shipCount++;
    }

    player.shipHead = shipHead;
    return player;
}


Ship *putShip(Ship *head, int count, int width, char map[10][10]) {
    for (int i = count; i > 0; --i) {
        showMap(map);
        printf("putting a ship with width of %d and height of 1\n", width);
//        getting start coordinate
        Coordinate startCoord;
        printf("please input start x:\t");
        scanf("%d", &(startCoord.x));
        printf("please input start y:\t");
        scanf("%d", &(startCoord.y));
        Coordinate endCoord;

        if (width != 1) {
            //        getting end coordinate
            printf("please input end x:\t");
            scanf("%d", &(endCoord.x));
            printf("please input end y:\t");
            scanf("%d", &(endCoord.y));
        }

        if (width == 1) {
            endCoord = startCoord;
        }

        while (!shipCanBePlacedInCoordinates(width, map, startCoord, endCoord)) {
            printf("SHIP CAN'T BE PLACED IN GIVEN COORDINATES PLEASE TRY AGAIN\n");

            showMap(map);
            printf("putting a ship with width : %d and height : 1\n", width);

            Coordinate startCoord;
            printf("please input start x:\t");
            scanf("%d", &(startCoord.x));
            printf("please input start y:\t");
            scanf("%d", &(startCoord.y));

            Coordinate endCoord;
            printf("please input end x:\t");
            scanf("%d", &(endCoord.x));
            printf("please input end y:\t");
            scanf("%d", &(endCoord.y));
        }

        head = addShipToList(head, width, startCoord, endCoord);
        updateMap(map, startCoord, endCoord);
    }

    return head;
}

void updateMap(char map[10][10], Coordinate start, Coordinate end) {
    if (shipIsHorizontal(start, end)) {
//        their y is the same
        int lowestX = findLowestX(start, end);
        int highestX = findHighestX(start, end);

        for (int column = lowestX; column <= highestX; ++column) {
            map[start.y][column] = 'S';
        }

        return;
    }

// their x is the same
    int lowestY = findLowestY(start, end);
    int highestY = findHighestY(start, end);

    for (int row = lowestY; row <= highestY; ++row) {
        map[row][start.x] = 'S';
    }
}

bool shipIsHorizontal(Coordinate start, Coordinate end) {
    return start.y == end.y;
}

Ship *addShipToList(Ship *head, int width, Coordinate start, Coordinate end) {
    if (!head) {
        head = (Ship *) calloc(1, sizeof(Ship));
        head->start = start;
        head->end = end;
        head->width = width;
        head->hitCount = 0;
        head->next = NULL;

        return head;
    }

    Ship *newShip = (Ship *) calloc(1, sizeof(Ship));
    newShip->width = width;
    newShip->hitCount = 0;
    newShip->end = end;
    newShip->start = start;
    newShip->next = NULL;

    Ship *tale = findTale(head);
    tale->next = newShip;

    return head;
}

Ship *findTale(Ship *head) {
    Ship *iterationShip = head;

    while (iterationShip->next) {
        iterationShip = iterationShip->next;
    }

    return iterationShip;
}


bool shipIsVertical(Coordinate start, Coordinate end) {
    return start.x == end.x;
}


bool shipCanBePlacedInCoordinates(int shipWidth, char map[10][10], Coordinate start, Coordinate end) {
    if (isOutOfRange(start, end)) {
        return false;
    }

    if (shipWidth == 1) {
        if (map[start.x][start.y] == 'S') {
            return false;
        }

        return true;
    }

    if (shipIsHorizontal(start, end)) {
//        their y is the same
        int distance = abs(start.x - end.x) + 1;

        if (distance != shipWidth) {
            return false;
        }
    } else {
//        their x is the same
        int distance = abs(start.y - end.y) + 1;

        if (distance != shipWidth) {
            return false;
        }
    }

    if (!shipIsHorizontal(start, end) && !shipIsVertical(start, end)) {
//        ship must be horizontal or vertical
        return false;
    }


    int highestX = findHighestX(start, end);
    int lowestX = findLowestX(start, end);

    int lowestY = findLowestY(start, end);
    int highestY = findHighestY(start, end);

    for (int row = lowestX - 1; row <= highestX + 1; ++row) {
        if (row == -1) {
            row = 0;
        }
        if (row == 10) {
            break;
        }

        for (int column = lowestY - 1; column <= highestY + 1; ++column) {
            if (column == -1) {
                column = 0;
            }

            if (column == 10) {
                break;
            }

            if (map[column][row] == 'S') {
                return false;
            }
        }
    }

    return true;
}
